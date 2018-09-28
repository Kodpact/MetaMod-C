#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

#include "Types.h"
#include "UTIL.h"
#include "MetaMod.h"
#include "HashMap.h"

extern Plugin *Plugins;
extern int PluginCount;

extern MetaMsg Messages[MAX_USER_MESSAGES];
extern StringHashMap *MessageHashMap;

static hudtextparms_t DefaultHUDParams = { -1, 0.25, 2, 0, 255, 0, 0, 10, 0, 0, 0, 20, 0, 10, 10, 1 };
static char Buffer[1024];
static hudtextparms_t HUDParams;
static cvar_t *Developer;

void MetaFuncs_Init(void)
{
	Developer = EngineFuncs.pfnCVarGetPointer("developer");
}

static void MF_LogConsole(PluginInfo *Info, const char *Format, ...)
{
	va_list VList;
	va_start(VList, Format);
	int Bytes = vsnprintf(Buffer, 1023, Format, VList);
	va_end(VList);
	Buffer[Bytes] = '\n';
	Buffer[Bytes + 1] = '\0';
	SERVER_PRINT(Buffer);
}

static void MF_LogMessage(PluginInfo *Info, const char *Format, ...)
{
	va_list VList;
	va_start(VList, Format);
	int Bytes = vsnprintf(Buffer, 1023, Format, VList);
	va_end(VList);
	Buffer[Bytes] = '\n';
	Buffer[Bytes + 1] = '\0';
	ALERT(at_logged, "[%s] %s", Info->LogTag, Buffer);
}

static void MF_LogError(PluginInfo *Info, const char *Format, ...)
{
	va_list VList;
	va_start(VList, Format);
	int Bytes = vsnprintf(Buffer, 1023, Format, VList);
	va_end(VList);
	Buffer[Bytes] = '\n';
	Buffer[Bytes + 1] = '\0';
	ALERT(at_logged, "[%s] ERROR: %s", Info->LogTag, Buffer);
}

static void MF_LogDeveloper(PluginInfo *Info, const char *Format, ...)
{
	if (!Developer->value)
		return;

	va_list VList;
	va_start(VList, Format);
	int Bytes = vsnprintf(Buffer, 1023, Format, VList);
	va_end(VList);
	Buffer[Bytes] = '\n';
	Buffer[Bytes + 1] = '\0';
	ALERT(at_logged, "[%s] DEV: %s", Info->LogTag, Buffer);
}

static void MF_HudMessage(edict_t *Entity, char *Msg)
{
	if (fast_FNullEnt(Entity) || Entity->free)
		return;

	MESSAGE_BEGIN(MSG_ONE, SVC_TEMPENTITY, NULL, Entity);
	WRITE_BYTE(TE_TEXTMESSAGE);
	WRITE_BYTE(HUDParams.channel & 0xFF);
	WRITE_SHORT(FixedSigned16(HUDParams.x, 1 << 13));
	WRITE_SHORT(FixedSigned16(HUDParams.y, 1 << 13));
	WRITE_BYTE(HUDParams.effect);
	WRITE_BYTE(HUDParams.r1);
	WRITE_BYTE(HUDParams.g1);
	WRITE_BYTE(HUDParams.b1);
	WRITE_BYTE(HUDParams.a1);
	WRITE_BYTE(HUDParams.r2);
	WRITE_BYTE(HUDParams.g2);
	WRITE_BYTE(HUDParams.b2);
	WRITE_BYTE(HUDParams.a2);
	WRITE_SHORT(FixedUnsigned16(HUDParams.fadeinTime, 1 << 8));
	WRITE_SHORT(FixedUnsigned16(HUDParams.fadeoutTime, 1 << 8));
	WRITE_SHORT(FixedUnsigned16(HUDParams.holdTime, 1 << 8));

	if (HUDParams.effect == 2)
		WRITE_SHORT(FixedUnsigned16(HUDParams.fxTime, 1 << 8));

	if (strlen(Msg) > 511)
		Msg[511] = 0;

	WRITE_STRING(Msg);
	MESSAGE_END();
}

static void MF_CenterSayVarargs(PluginInfo *Info, hudtextparms_t HParams, const char *Format, va_list VList)
{
	int Length = vsnprintf(Buffer, 1022, Format, VList);
	int WriteAmount = min(1022 - Length, 16) + Length;

	if (WriteAmount > 0)
	{
		memmove(Buffer + WriteAmount, Buffer, Length);
		strcpy(Buffer, "(CenterSay) ");
	}

	HUDParams = HParams;
	int Index = 1;

	for (Index; Index <= GlobalVars->maxClients; Index++)
		MF_HudMessage(EngineFuncs.pfnPEntityOfEntIndex(Index), Buffer);
}

static void MF_CenterSay(PluginInfo *Info, const char *Format, ...)
{
	va_list VList;
	va_start(VList, Format);
	MF_CenterSayVarargs(Info, DefaultHUDParams, Format, VList);
	va_end(VList);
}

static void MF_CenterSayParms(PluginInfo *Info, hudtextparms_t HParams, const char *Format, ...)
{
	va_list VList;
	va_start(VList, Format);
	MF_CenterSayVarargs(Info, HParams, Format, VList);
	va_end(VList);
}

static qboolean MF_CallGameEntity(PluginInfo *Info, const char *ClassName, entvars_t *EVars)
{
	ENTITY_FN FEntity;
	FEntity = (ENTITY_FN)GetProcAddress(GameDLL, ClassName);

	if (!FEntity)
		return FALSE;

	FEntity(EVars);
	return TRUE;
}

static int MF_GetUserMsgID(PluginInfo *Info, const char *Name, int *Size)
{
	int Index;
	
	if (HM_Retrieve(MessageHashMap, Name, &Index))
	{
		if (Size)
			*Size = Messages[Index - 64].Size;
		return Index;
	}
	else
		return 0;
}

static const char *MF_GetUserMsgName(PluginInfo *Info, int MsgID, int *Size)
{
	if (MsgID < 64)
		return "";
	
	MetaMsg *CurMsg = &Messages[MsgID - 64];
	*Size = CurMsg->Size;
	return CurMsg->Name;
}

static const char *MF_GetPluginPath(PluginInfo *Info)
{
	for (int Index = 0; Index < PluginCount; Index++)
	{
		if (Plugins[Index].Info == Info)
			return Plugins[Index].PathName;
	}

	return NULL;
}

extern GameInfo DLLInfo;

static const char *MF_GetGameInfo(PluginInfo *Info, GameInfoE GInfo)
{
	switch (GInfo)
	{
		case GINFO_NAME: return DLLInfo.Name;
		case GINFO_DESC: return DLLInfo.Desc;
		case GINFO_GAMEDIR: return DLLInfo.GameDir;
		case GINFO_DLL_FULLPATH: return DLLInfo.DllFullPath;
		case GINFO_DLL_FILENAME: return DLLInfo.DllFileName;
		case GINFO_REALDLL_FULLPATH: return DLLInfo.RealDllFullPath;
	}

	return NULL;
}

extern Plugin *Plugins;
extern int PluginCount;

extern PluginFile *PluginFiles;
extern int PluginFileCount;

static int MF_LoadMetaPlugin(PluginInfo *Info, const char *Path, PLUG_LOADTIME LT, void **Handle)
{
	int Len = strlen(Path);
	char *PluginPath = malloc(Len + 1);
	
	if (!PluginPath)
		return -1;

	strcpy(PluginPath, Path);
	LoadPlugin(PluginPath, PT_STARTUP);
	*Handle = Plugins[PluginCount - 1].Handle;
	return 0;
}

static int MF_UnloadMetaPlugin(PluginInfo *Info, const char *Name, PLUG_LOADTIME Now, PL_UNLOAD_REASON Reason)
{
	MetaMod_SetPluginStatus(PL_PAUSED);
	return 0;
}

static int MF_UnloadMetaPluginByHandle(PluginInfo *Info, void *Name, PLUG_LOADTIME Now, PL_UNLOAD_REASON Reason)
{
	MetaMod_SetPluginStatus(PL_PAUSED);
	return 0;
}

static const char *MF_IsQueryingClientCvar(PluginInfo *Info, const edict_t *player)
{
	return "";
}

static int MF_MakeRequestID(PluginInfo *Info)
{
	static int RequestIDCounter = 0;
	return (abs(0xbeef << 16) + (++RequestIDCounter));
}

static void MF_GetHookTables(PluginInfo *Info, enginefuncs_t **EngineFuncs, DLL_FUNCTIONS **DLLFuncs, NEW_DLL_FUNCTIONS **NewDLLFuncs)
{
	if (EngineFuncs)
		*EngineFuncs = &MetaEngineFuncs;

	if (DLLFuncs)
		*DLLFuncs = &MetaDLLFuncTable;

	if (NewDLLFuncs)
		*NewDLLFuncs = &MetaDLLNewFuncTable;
}

META_FUNCS MetaFuncs =
{
	MF_LogConsole,		// pfnLogConsole
	MF_LogMessage,		// pfnLogMessage
	MF_LogError,			// pfnLogError
	MF_LogDeveloper,		// pfnLogDeveloper
	MF_CenterSay,		// pfnCenterSay
	MF_CenterSayParms,	// pfnCenterSayParms
	MF_CenterSayVarargs,	// pfnCenterSayVarargs
	MF_CallGameEntity,	// pfnCallGameEntity
	MF_GetUserMsgID,		// pfnGetUserMsgID
	MF_GetUserMsgName,	// pfnGetUserMsgName
	MF_GetPluginPath,	// pfnGetPluginPath
	MF_GetGameInfo,		// pfnGetGameInfo
	MF_LoadMetaPlugin, // pfnLoadPlugin
	MF_UnloadMetaPlugin, // pfnUnloadPlugin
	MF_UnloadMetaPluginByHandle, // pfnUnloadPluginByHandle
	MF_IsQueryingClientCvar, // pfnIsQueryingClientCvar
	MF_MakeRequestID, 	// pfnMakeRequestID
	MF_GetHookTables,   // pfnGetHookTables
};