#include <stddef.h>
#include <stdio.h>
#include <Windows.h>

#include "Types.h"
#include "UTIL.h"
#include "MetaFuncs.h"
#include "HashMap.h"

#undef NEAR

extern HMODULE GameDLL;
extern int RequestIDCounter;

DLL_FUNCTIONS GameDLLFuncs;
NEW_DLL_FUNCTIONS GameNewDLLFuncs;
NEW_DLL_FUNCTIONS **PluginDLLFuncs = NULL;
NEW_DLL_FUNCTIONS **PluginDLLNewFuncs = NULL;
enginefuncs_t EngineFuncs;
enginefuncs_t PluginEngineFuncs;
globalvars_t *GlobalVars;
MetaGlobals MGlobals;
Plugin *Plugins = NULL;
int PluginCount = 0;
static va_list Buformat;
static char Buffer[2048];
MetaMsg Messages[MAX_USER_MESSAGES];
StringHashMap *MessageHashMap;
StringHashMap *CVarHashMap;
StringHashMap *CmdHashMap;
const int SIZEOF_DLL = sizeof(DLL_FUNCTIONS) / 4;
const int SIZEOF_DLL_NEW = sizeof(NEW_DLL_FUNCTIONS) / 4;
const int SIZEOF_ENG = sizeof(enginefuncs_t) / 4;
const int PLUGIN_SIZE = sizeof(Plugin);

static int Dest;
static int Ofs;
static int Count;
static int RetVal;
static int RetValOver;

static CallGameFunction()
{
	int Index, Andex, Result;
	int CurrentDest = Dest;
	int CurrentOfs = Ofs;
	int CurrentCount = Count;
	int CountStack = Count * 4;
	BOOL SkipReal = FALSE;
	BOOL OverrideResult = FALSE;

	F Function;
	MGlobals.Result = MRES_UNSET;

	for (Index = 0; Index < PluginCount; Index++)
	{
		if (Plugins[Index].Status != PL_RUNNING)
			continue;

		__asm
		{
			IMUL EAX, Index, 88
				ADD EAX, DWORD PTR Plugins
				IMUL ECX, CurrentDest, 4
				MOV ECX, DWORD PTR[EAX + ECX + 4]
				MOV EAX, CurrentOfs
				MOV EAX, DWORD PTR[ECX + EAX]
				MOV Function, EAX
		}

		if (!Function)
			continue;

		for (Andex = CurrentCount; Andex > 0; Andex--)
		{
			__asm MOV EAX, Andex
			__asm PUSH[EBP + EAX * 4 + 4];
		}

		__asm CALL Function
		__asm ADD ESP, CountStack
		__asm MOV Result, EAX

		if (MGlobals.Result > MRES_OVERRIDE)
			SkipReal = TRUE;

		if (MGlobals.Result > MRES_HANDLED)
		{
			OverrideResult = TRUE;
			__asm MOV EAX, Result;
			__asm MOV RetValOver, EAX;
		}
	}

	if (SkipReal)
		goto MidLoop_Ignore;

	for (Andex = CurrentCount; Andex > 0; Andex--)
	{
		__asm MOV EAX, Andex
		__asm PUSH[EBP + EAX * 4 + 4];
	}

	__asm
	{
		MOV EAX, DWORD PTR CurrentOfs
			CMP CurrentDest, 1
			JE SHORT MidLoop_NewDLLFuncs
			JG SHORT MidLoop_Engine
			ADD EAX, OFFSET GameDLLFuncs
			JMP SHORT MidLoop_Continue
		MidLoop_NewDLLFuncs :
		ADD EAX, OFFSET GameNewDLLFuncs
			JMP SHORT MidLoop_Continue
		MidLoop_Engine :
		ADD EAX, OFFSET EngineFuncs
		MidLoop_Continue :
		MOV EAX, DWORD PTR[EAX]
			MOV Function, EAX
	}

	if (Function)
	{
		Function();

		if (!OverrideResult)
			__asm MOV Result, EAX

		__asm MOV RetVal, EAX
	}

	__asm ADD ESP, CountStack

MidLoop_Ignore :

	for (Index = 0; Index < PluginCount; Index++)
	{
		if (Plugins[Index].Status != PL_RUNNING)
			continue;

		__asm
		{
			IMUL EAX, Index, 88
				ADD EAX, DWORD PTR Plugins
				IMUL ECX, CurrentDest, 4
				MOV ECX, DWORD PTR[EAX + ECX + 16]
				MOV EAX, CurrentOfs
				MOV EAX, DWORD PTR[ECX + EAX]
				MOV Function, EAX
		}

		if (!Function)
			continue;

		for (Andex = CurrentCount; Andex > 0; Andex--)
		{
			__asm MOV EAX, Andex
			__asm PUSH[EBP + EAX * 4 + 4];
		}

		__asm CALL Function
		__asm ADD ESP, CountStack
	}

	if (OverrideResult)
		__asm MOV EAX, RetValOver
	else
		__asm MOV EAX, RetVal
}

__declspec(naked) pfnGameInit() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnGameInit); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSpawn() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnSpawn); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnThink() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnThink); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnUse() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnUse); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnTouch() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnTouch); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnBlocked() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnBlocked); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnKeyValue() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnKeyValue); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSave() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnSave); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnRestore() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnRestore); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetAbsBox() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnSetAbsBox); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSaveWriteFields() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnSaveWriteFields); Count = 5; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSaveReadFields() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnSaveReadFields); Count = 5; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSaveGlobalState() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnSaveGlobalState); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnRestoreGlobalState() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnRestoreGlobalState); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnResetGlobalState() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnResetGlobalState); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnClientConnect() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnClientConnect); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnClientDisconnect() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnClientDisconnect); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnClientKill() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnClientKill); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnClientPutInServer() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnClientPutInServer); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnClientCommand() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnClientCommand); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnClientUserInfoChanged() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnClientUserInfoChanged); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnServerActivate() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnServerActivate); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPlayerPreThink() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnPlayerPreThink); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPlayerPostThink() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnPlayerPostThink); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnStartFrame() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnStartFrame); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnParmsNewLevel() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnParmsNewLevel); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnParmsChangeLevel() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnParmsChangeLevel); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetGameDescription() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnGetGameDescription); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPlayerCustomization() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnPlayerCustomization); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSpectatorConnect() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnSpectatorConnect); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSpectatorDisconnect() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnSpectatorDisconnect); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSpectatorThink() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnSpectatorThink); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSys_Error() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnSys_Error); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPM_Move() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnPM_Move); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPM_Init() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnPM_Init); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPM_FindTextureType() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnPM_FindTextureType); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetupVisibility() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnSetupVisibility); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnUpdateClientData() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnUpdateClientData); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnAddToFullPack() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnAddToFullPack); Count = 7; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCreateBaseline() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnCreateBaseline); Count = 7; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnRegisterEncoders() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnRegisterEncoders); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetWeaponData() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnGetWeaponData); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCmdStart() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnCmdStart); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCmdEnd() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnCmdEnd); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnConnectionlessPacket() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnConnectionlessPacket); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetHullBounds() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnGetHullBounds); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCreateInstancedBaselines() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnCreateInstancedBaselines); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnInconsistentFile() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnInconsistentFile); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnAllowLagCompensation() { Dest = DEST_DLLFUNC; Ofs = offsetof(DLL_FUNCTIONS, pfnAllowLagCompensation); Count = 0; __asm { JMP CallGameFunction }; }

static pfnServerDeactivate()
{
	Dest = DEST_DLLFUNC;
	Ofs = offsetof(DLL_FUNCTIONS, pfnServerDeactivate);
	Count = 0;
	CallGameFunction();

	for (int Index = 0; Index < PluginCount; Index++)
	{
		if (Plugins[Index].Status == PL_PAUSED)
			Plugins[Index].Status = PL_RUNNING;
	}

	RequestIDCounter = 0;
}

DLL_FUNCTIONS MetaDLLFuncTable =
{
	pfnGameInit,
	pfnSpawn,
	pfnThink,
	pfnUse,
	pfnTouch,
	pfnBlocked,
	pfnKeyValue,
	pfnSave,
	pfnRestore,
	pfnSetAbsBox,
	pfnSaveWriteFields,
	pfnSaveReadFields,
	pfnSaveGlobalState,
	pfnRestoreGlobalState,
	pfnResetGlobalState,
	pfnClientConnect,
	pfnClientDisconnect,
	pfnClientKill,
	pfnClientPutInServer,
	pfnClientCommand,
	pfnClientUserInfoChanged,
	pfnServerActivate,
	pfnServerDeactivate,
	pfnPlayerPreThink,
	pfnPlayerPostThink,
	pfnStartFrame,
	pfnParmsNewLevel,
	pfnParmsChangeLevel,
	pfnGetGameDescription,
	pfnPlayerCustomization,
	pfnSpectatorConnect,
	pfnSpectatorDisconnect,
	pfnSpectatorThink,
	pfnSys_Error,
	pfnPM_Move,
	pfnPM_Init,
	pfnPM_FindTextureType,
	pfnSetupVisibility,
	pfnUpdateClientData,
	pfnAddToFullPack,
	pfnCreateBaseline,
	pfnRegisterEncoders,
	pfnGetWeaponData,
	pfnCmdStart,
	pfnCmdEnd,
	pfnConnectionlessPacket,
	pfnGetHullBounds,
	pfnCreateInstancedBaselines,
	pfnInconsistentFile,
	pfnAllowLagCompensation
};

__declspec(naked) pfnPrecacheModel() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnPrecacheModel); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPrecacheSound() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnPrecacheSound); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetModel() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSetModel); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnModelIndex() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnModelIndex); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnModelFrames() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnModelFrames); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetSize() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSetSize); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnChangeLevel() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnChangeLevel); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetSpawnParms() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetSpawnParms); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSaveSpawnParms() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSaveSpawnParms); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnVecToYaw() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnVecToYaw); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnVecToAngles() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnVecToAngles); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnMoveToOrigin() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnMoveToOrigin); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnChangeYaw() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnChangeYaw); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnChangePitch() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnChangePitch); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnFindEntityByString() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnFindEntityByString); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetEntityIllum() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetEntityIllum); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnFindEntityInSphere() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnFindEntityInSphere); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnFindClientInPVS() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnFindClientInPVS); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnEntitiesInPVS() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnEntitiesInPVS); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnMakeVectors() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnMakeVectors); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnAngleVectors() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnAngleVectors); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCreateEntity() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCreateEntity); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnRemoveEntity() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnRemoveEntity); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCreateNamedEntity() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCreateNamedEntity); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnMakeStatic() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnMakeStatic); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnEntIsOnFloor() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnEntIsOnFloor); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnDropToFloor() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnDropToFloor); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnWalkMove() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnWalkMove); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetOrigin() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSetOrigin); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnEmitSound() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnEmitSound); Count = 7; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnEmitAmbientSound() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnEmitAmbientSound); Count = 7; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnTraceLine() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnTraceLine); Count = 5; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnTraceToss() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnTraceToss); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnTraceMonsterHull() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnTraceMonsterHull); Count = 6; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnTraceHull() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnTraceHull); Count = 6; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnTraceModel() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnTraceModel); Count = 5; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnTraceTexture() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnTraceTexture); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnTraceSphere() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnTraceSphere); Count = 6; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetAimVector() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetAimVector); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnServerCommand() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnServerCommand); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnServerExecute() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnServerExecute); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnParticleEffect() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnParticleEffect); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnLightStyle() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnLightStyle); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnDecalIndex() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnDecalIndex); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPointContents() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnPointContents); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnMessageBegin() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnMessageBegin); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnMessageEnd() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnMessageEnd); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnWriteByte() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnWriteByte); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnWriteChar() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnWriteChar); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnWriteShort() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnWriteShort); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnWriteLong() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnWriteLong); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnWriteAngle() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnWriteAngle); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnWriteCoord() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnWriteCoord); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnWriteString() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnWriteString); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnWriteEntity() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnWriteEntity); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCVarRegister() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCVarRegister); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCVarGetFloat() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCVarGetFloat); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCVarGetString() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCVarGetString); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCVarSetFloat() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCVarSetFloat); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCVarSetString() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCVarSetString); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPvAllocEntPrivateData() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnPvAllocEntPrivateData); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPvEntPrivateData() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnPvEntPrivateData); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnFreeEntPrivateData() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnFreeEntPrivateData); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSzFromIndex() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSzFromIndex); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnAllocString() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnAllocString); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetVarsOfEnt() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetVarsOfEnt); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPEntityOfEntOffset() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnPEntityOfEntOffset); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnEntOffsetOfPEntity() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnEntOffsetOfPEntity); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnIndexOfEdict() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnIndexOfEdict); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPEntityOfEntIndex() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnPEntityOfEntIndex); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnFindEntityByVars() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnFindEntityByVars); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetModelPtr() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetModelPtr); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnAnimationAutomove() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnAnimationAutomove); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetBonePosition() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetBonePosition); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnFunctionFromName() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnFunctionFromName); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnNameForFunction() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnNameForFunction); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnClientSERVER_PRINT() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnClientSERVER_PRINT); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnServerPrint() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnServerPrint); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCmd_Args() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCmd_Args); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCmd_Argv() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCmd_Argv); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCmd_Argc() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCmd_Argc); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetAttachment() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetAttachment); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCRC32_Init() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCRC32_Init); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCRC32_ProcessBuffer() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCRC32_ProcessBuffer); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCRC32_ProcessByte() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCRC32_ProcessByte); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCRC32_Final() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCRC32_Final); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnRandomLong() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnRandomLong); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnRandomFloat() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnRandomFloat); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetView() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSetView); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnTime() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnTime); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCrosshairAngle() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCrosshairAngle); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnLoadFileForMe() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnLoadFileForMe); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnFreeFile() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnFreeFile); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnEndSection() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnEndSection); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCompareFileTime() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCompareFileTime); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetGameDir() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetGameDir); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCvar_RegisterVariable() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCvar_RegisterVariable); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnFadeClientVolume() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnFadeClientVolume); Count = 5; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetClientMaxspeed() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSetClientMaxspeed); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCreateFakeClient() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCreateFakeClient); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnNumberOfEntities() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnNumberOfEntities); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetInfoKeyBuffer() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetInfoKeyBuffer); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnInfoKeyValue() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnInfoKeyValue); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetKeyValue() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSetKeyValue); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetClientKeyValue() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSetClientKeyValue); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnIsMapValid() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnIsMapValid); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnStaticDecal() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnStaticDecal); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPrecacheGeneric() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnPrecacheGeneric); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetPlayerUserId() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetPlayerUserId); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnBuildSoundMsg() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnBuildSoundMsg); Count = 11; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnIsDedicatedServer() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnIsDedicatedServer); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCVarGetPointer() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCVarGetPointer); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetPlayerWONId() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetPlayerWONId); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnInfo_RemoveKey() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnInfo_RemoveKey); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetPhysicsKeyValue() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetPhysicsKeyValue); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetPhysicsKeyValue() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSetPhysicsKeyValue); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetPhysicsInfoString() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetPhysicsInfoString); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPrecacheEvent() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnPrecacheEvent); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnPlaybackEvent() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnPlaybackEvent); Count = 12; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetFatPVS() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSetFatPVS); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetFatPAS() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSetFatPAS); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCheckVisibility() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCheckVisibility); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnDeltaSetField() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnDeltaSetField); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnDeltaUnsetField() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnDeltaUnsetField); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnDeltaAddEncoder() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnDeltaAddEncoder); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetCurrentPlayer() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetCurrentPlayer); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCanSkipPlayer() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCanSkipPlayer); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnDeltaFindField() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnDeltaFindField); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnDeltaSetFieldByIndex() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnDeltaSetFieldByIndex); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnDeltaUnsetFieldByIndex() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnDeltaUnsetFieldByIndex); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSetGroupMask() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSetGroupMask); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCreateInstancedBaseline() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCreateInstancedBaseline); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCvar_DirectSet() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCvar_DirectSet); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnForceUnmodified() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnForceUnmodified); Count = 4; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetPlayerStats() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetPlayerStats); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnAddServerCommand() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnAddServerCommand); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnVoice_GetClientListening() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnVoice_GetClientListening); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnVoice_SetClientListening() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnVoice_SetClientListening); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetPlayerAuthId() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetPlayerAuthId); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSequenceGet() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSequenceGet); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnSequencePickSentence() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnSequencePickSentence); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetFileSize() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetFileSize); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetApproxWavePlayLen() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetApproxWavePlayLen); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnIsCareerMatch() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnIsCareerMatch); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetLocalizedStringLength() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetLocalizedStringLength); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnRegisterTutorMessageShown() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnRegisterTutorMessageShown); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGetTimesTutorMessageShown() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnGetTimesTutorMessageShown); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnProcessTutorMessageDecayBuffer() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnProcessTutorMessageDecayBuffer); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnConstructTutorMessageDecayBuffer() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnConstructTutorMessageDecayBuffer); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnResetTutorMessageDecayData() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnResetTutorMessageDecayData); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnQueryClientCvarValue() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnQueryClientCvarValue); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnQueryClientCvarValue2() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnQueryClientCvarValue2); Count = 3; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCheckParm() { Dest = DEST_ENGINE; Ofs = offsetof(enginefuncs_t, pfnCheckParm); Count = 2; __asm { JMP CallGameFunction }; }

static pfnRegUserMsg(const char *Name, int Size)
{
	Dest = DEST_ENGINE;
	Ofs = offsetof(enginefuncs_t, pfnRegUserMsg);
	Count = 2;
	int Index = CallGameFunction(Name, Size);

	if (Index > 0)
	{
		MetaMsg *CurMsg = &Messages[Index - 64];
		CurMsg->Name = Name;
		CurMsg->Size = Size;
		CurMsg->Index = Index;
		HM_Insert(MessageHashMap, Name, Index);
	}

	return Index;
}

static pfnAlertMessage(int A1, const char *A2, ...)
{
	Dest = DEST_ENGINE;
	Ofs = offsetof(enginefuncs_t, pfnAlertMessage);
	Count = 2;
	va_start(Buformat, A2);
	vsprintf(Buffer, A2, Buformat);
	va_end(Buformat);
	CallGameFunction(A1, Buffer);
}

static pfnEngineFprintf(int A1, const char *A2, ...)
{
	Dest = DEST_ENGINE;
	Ofs = offsetof(enginefuncs_t, pfnEngineFprintf);
	Count = 2;
	va_start(Buformat, A2);
	vsprintf(Buffer, A2, Buformat);
	va_end(Buformat);
	CallGameFunction(A1, Buffer);
}

static pfnClientCommandEng(int A1, const char *A2, ...)
{
	Dest = DEST_ENGINE;
	Ofs = offsetof(enginefuncs_t, pfnClientCommand);
	Count = 2;
	va_start(Buformat, A2);
	vsprintf(Buffer, A2, Buformat);
	va_end(Buformat);
	CallGameFunction(A1, Buffer);
}

static pfnRunPlayerMove(int A1, int A2, int A3, int A4, int A5, byte A6, byte A7, byte A8, byte A9)
{
	Dest = DEST_ENGINE;
	Ofs = offsetof(enginefuncs_t, pfnRunPlayerMove);
	Count = 6;
	CallGameFunction(A1, A2, A3, A4, A5, (int)(A6 << 24 | A7 << 16 | A8 << 8 | A9));
}

enginefuncs_t MetaEngineFuncs =
{
	&pfnPrecacheModel,			// pfnPrecacheModel()
	&pfnPrecacheSound,			// pfnPrecacheSound()
	&pfnSetModel,				// pfnSetModel()
	&pfnModelIndex,				// pfnModelIndex()
	&pfnModelFrames,			// pfnModelFrames()

	&pfnSetSize,				// pfnSetSize()
	&pfnChangeLevel,			// pfnChangeLevel()
	&pfnGetSpawnParms,			// pfnGetSpawnParms()
	&pfnSaveSpawnParms,			// pfnSaveSpawnParms()

	&pfnVecToYaw,				// pfnVecToYaw()
	&pfnVecToAngles,			// pfnVecToAngles()
	&pfnMoveToOrigin,			// pfnMoveToOrigin()
	&pfnChangeYaw,				// pfnChangeYaw()
	&pfnChangePitch,			// pfnChangePitch()

	&pfnFindEntityByString,		// pfnFindEntityByString()
	&pfnGetEntityIllum,			// pfnGetEntityIllum()
	&pfnFindEntityInSphere,		// pfnFindEntityInSphere()
	&pfnFindClientInPVS,		// pfnFindClientInPVS()
	&pfnEntitiesInPVS,			// pfnEntitiesInPVS()

	&pfnMakeVectors,			// pfnMakeVectors()
	&pfnAngleVectors,			// pfnAngleVectors()

	&pfnCreateEntity,			// pfnCreateEntity()
	&pfnRemoveEntity,			// pfnRemoveEntity()
	&pfnCreateNamedEntity,		// pfnCreateNamedEntity()

	&pfnMakeStatic,				// pfnMakeStatic()
	&pfnEntIsOnFloor,			// pfnEntIsOnFloor()
	&pfnDropToFloor,			// pfnDropToFloor()

	&pfnWalkMove,				// pfnWalkMove()
	&pfnSetOrigin,				// pfnSetOrigin()

	&pfnEmitSound,				// pfnEmitSound()
	&pfnEmitAmbientSound,		// pfnEmitAmbientSound()

	&pfnTraceLine,				// pfnTraceLine()
	&pfnTraceToss,				// pfnTraceToss()
	&pfnTraceMonsterHull,		// pfnTraceMonsterHull()
	&pfnTraceHull,				// pfnTraceHull()
	&pfnTraceModel,				// pfnTraceModel()
	&pfnTraceTexture,			// pfnTraceTexture()
	&pfnTraceSphere,			// pfnTraceSphere()
	&pfnGetAimVector,			// pfnGetAimVector()

	&pfnServerCommand,			// pfnServerCommand()
	&pfnServerExecute,			// pfnServerExecute()
	&pfnClientCommandEng,		// pfnClientCommand()	// D'oh, ClientCommand in dllapi too.

	&pfnParticleEffect,			// pfnParticleEffect()
	&pfnLightStyle,				// pfnLightStyle()
	&pfnDecalIndex,				// pfnDecalIndex()
	&pfnPointContents,			// pfnPointContents()

	&pfnMessageBegin,			// pfnMessageBegin()
	&pfnMessageEnd,				// pfnMessageEnd()

	&pfnWriteByte,				// pfnWriteByte()
	&pfnWriteChar,				// pfnWriteChar()
	&pfnWriteShort,				// pfnWriteShort()
	&pfnWriteLong,				// pfnWriteLong()
	&pfnWriteAngle,				// pfnWriteAngle()
	&pfnWriteCoord,				// pfnWriteCoord()
	&pfnWriteString,			// pfnWriteString()
	&pfnWriteEntity,			// pfnWriteEntity()

	&pfnCVarRegister,			// pfnCVarRegister()
	&pfnCVarGetFloat,			// pfnCVarGetFloat()
	&pfnCVarGetString,			// pfnCVarGetString()
	&pfnCVarSetFloat,			// pfnCVarSetFloat()
	&pfnCVarSetString,			// pfnCVarSetString()

	&pfnAlertMessage,			// pfnAlertMessage()
	&pfnEngineFprintf,			// pfnEngineFprintf()

	&pfnPvAllocEntPrivateData,	// pfnPvAllocEntPrivateData()
	&pfnPvEntPrivateData,		// pfnPvEntPrivateData()
	&pfnFreeEntPrivateData,		// pfnFreeEntPrivateData()

	&pfnSzFromIndex,			// pfnSzFromIndex()
	&pfnAllocString,			// pfnAllocString()

	&pfnGetVarsOfEnt, 			// pfnGetVarsOfEnt()
	&pfnPEntityOfEntOffset,		// pfnPEntityOfEntOffset()
	&pfnEntOffsetOfPEntity,		// pfnEntOffsetOfPEntity()
	&pfnIndexOfEdict,			// pfnIndexOfEdict()
	&pfnPEntityOfEntIndex,		// pfnPEntityOfEntIndex()
	&pfnFindEntityByVars,		// pfnFindEntityByVars()
	&pfnGetModelPtr,			// pfnGetModelPtr()

	&pfnRegUserMsg,				// pfnRegUserMsg()

	&pfnAnimationAutomove,		// pfnAnimationAutomove()
	&pfnGetBonePosition,		// pfnGetBonePosition()

	&pfnFunctionFromName,		// pfnFunctionFromName()
	&pfnNameForFunction,		// pfnNameForFunction()

	&pfnClientSERVER_PRINT,			// pfnClientSERVER_PRINT()			//! JOHN: engine callbacks so game DLL can print messages to individual clients
	&pfnServerPrint,			// pfnServerPrint()

	&pfnCmd_Args,				// pfnCmd_Args()				//! these 3 added 
	&pfnCmd_Argv,				// pfnCmd_Argv()				//! so game DLL can easily 
	&pfnCmd_Argc,				// pfnCmd_Argc()				//! access client 'cmd' strings

	&pfnGetAttachment,			// pfnGetAttachment()

	&pfnCRC32_Init,				// pfnCRC32_Init()
	&pfnCRC32_ProcessBuffer,	// pfnCRC32_ProcessBuffer()
	&pfnCRC32_ProcessByte,		// pfnCRC32_ProcessByte()
	&pfnCRC32_Final,			// pfnCRC32_Final()

	&pfnRandomLong,				// pfnRandomLong()
	&pfnRandomFloat,			// pfnRandomFloat()

	&pfnSetView,				// pfnSetView()
	&pfnTime,					// pfnTime()
	&pfnCrosshairAngle,			// pfnCrosshairAngle()

	&pfnLoadFileForMe,			// pfnLoadFileForMe()
	&pfnFreeFile,				// pfnFreeFile()

	&pfnEndSection,				// pfnEndSection()				//! trigger_endsection
	&pfnCompareFileTime,		// pfnCompareFileTime()
	&pfnGetGameDir,				// pfnGetGameDir()
	&pfnCvar_RegisterVariable,	// pfnCvar_RegisterVariable()
	&pfnFadeClientVolume,		// pfnFadeClientVolume()
	&pfnSetClientMaxspeed,		// pfnSetClientMaxspeed()
	&pfnCreateFakeClient,		// pfnCreateFakeClient() 		//! returns NULL if fake client can't be created
	&pfnRunPlayerMove,			// pfnRunPlayerMove()
	&pfnNumberOfEntities,		// pfnNumberOfEntities()

	&pfnGetInfoKeyBuffer,		// pfnGetInfoKeyBuffer()		//! passing in NULL gets the serverinfo
	&pfnInfoKeyValue,			// pfnInfoKeyValue()
	&pfnSetKeyValue,			// pfnSetKeyValue()
	&pfnSetClientKeyValue,		// pfnSetClientKeyValue()

	&pfnIsMapValid,				// pfnIsMapValid()
	&pfnStaticDecal,			// pfnStaticDecal()
	&pfnPrecacheGeneric,		// pfnPrecacheGeneric()
	&pfnGetPlayerUserId, 		// pfnGetPlayerUserId()			//! returns the server assigned userid for this player.
	&pfnBuildSoundMsg,			// pfnBuildSoundMsg()
	&pfnIsDedicatedServer,		// pfnIsDedicatedServer()		//! is this a dedicated server?
	&pfnCVarGetPointer,			// pfnCVarGetPointer()
	&pfnGetPlayerWONId,			// pfnGetPlayerWONId()			//! returns the server assigned WONid for this player.

	//! YWB 8/1/99 TFF Physics additions
	&pfnInfo_RemoveKey,			// pfnInfo_RemoveKey()
	&pfnGetPhysicsKeyValue,		// pfnGetPhysicsKeyValue()
	&pfnSetPhysicsKeyValue,		// pfnSetPhysicsKeyValue()
	&pfnGetPhysicsInfoString,	// pfnGetPhysicsInfoString()
	&pfnPrecacheEvent,			// pfnPrecacheEvent()
	&pfnPlaybackEvent,			// pfnPlaybackEvent()

	&pfnSetFatPVS,				// pfnSetFatPVS()
	&pfnSetFatPAS,				// pfnSetFatPAS()

	&pfnCheckVisibility,		// pfnCheckVisibility()

	&pfnDeltaSetField,			// pfnDeltaSetField()
	&pfnDeltaUnsetField,		// pfnDeltaUnsetField()
	&pfnDeltaAddEncoder,		// pfnDeltaAddEncoder()
	&pfnGetCurrentPlayer,		// pfnGetCurrentPlayer()
	&pfnCanSkipPlayer,			// pfnCanSkipPlayer()
	&pfnDeltaFindField,			// pfnDeltaFindField()
	&pfnDeltaSetFieldByIndex,	// pfnDeltaSetFieldByIndex()
	&pfnDeltaUnsetFieldByIndex,	// pfnDeltaUnsetFieldByIndex()

	&pfnSetGroupMask,			// pfnSetGroupMask()

	&pfnCreateInstancedBaseline, // pfnCreateInstancedBaseline()		// D'oh, CreateInstancedBaseline in dllapi too.
	&pfnCvar_DirectSet,			// pfnCvar_DirectSet()

	&pfnForceUnmodified,		// pfnForceUnmodified()

	&pfnGetPlayerStats,			// pfnGetPlayerStats()

	&pfnAddServerCommand,		// pfnAddServerCommand()

	// Added in SDK 2l2:
	&pfnVoice_GetClientListening,	// pfnVoice_GetClientListening()
	&pfnVoice_SetClientListening,	// pfnVoice_SetClientListening()

	// Added for HL 1109 (no SDK update):
	&pfnGetPlayerAuthId,			// pfnGetPlayerAuthId()

	// Added 2003/11/10 (no SDK update):
	&pfnSequenceGet,					// pfnSequenceGet()
	&pfnSequencePickSentence,			// pfnSequencePickSentence()
	&pfnGetFileSize,					// pfnGetFileSize()
	&pfnGetApproxWavePlayLen,			// pfnGetApproxWavePlayLen()
	&pfnIsCareerMatch,				// pfnIsCareerMatch()
	&pfnGetLocalizedStringLength,		// pfnGetLocalizedStringLength()
	&pfnRegisterTutorMessageShown,	// pfnRegisterTutorMessageShown()
	&pfnGetTimesTutorMessageShown,	// pfnGetTimesTutorMessageShown()
	&pfnProcessTutorMessageDecayBuffer,	// pfnProcessTutorMessageDecayBuffer()
	&pfnConstructTutorMessageDecayBuffer,	// pfnConstructTutorMessageDecayBuffer()
	&pfnResetTutorMessageDecayData,		// pfnResetTutorMessageDecayData()

	// Added 2005/08/11 (no SDK update):
	&pfnQueryClientCvarValue,		// pfnQueryClientCvarValue()

	// Added 2005/11/21 (no SDK update):
	&pfnQueryClientCvarValue2,		// pfnQueryClientCvarValue2()

	// Added 2009/06/17 (no SDK update):
	&pfnCheckParm			// pfnEngCheckParm()
};

__declspec(naked) pfnOnFreeEntPrivateData() { Dest = DEST_NEWDLLFUNC; Ofs = offsetof(NEW_DLL_FUNCTIONS, pfnOnFreeEntPrivateData); Count = 1; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnGameShutdown() { Dest = DEST_NEWDLLFUNC; Ofs = offsetof(NEW_DLL_FUNCTIONS, pfnGameShutdown); Count = 0; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnShouldCollide() { Dest = DEST_NEWDLLFUNC; Ofs = offsetof(NEW_DLL_FUNCTIONS, pfnShouldCollide); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCvarValue() { Dest = DEST_NEWDLLFUNC; Ofs = offsetof(NEW_DLL_FUNCTIONS, pfnCvarValue); Count = 2; __asm { JMP CallGameFunction }; }
__declspec(naked) pfnCvarValue2() { Dest = DEST_NEWDLLFUNC; Ofs = offsetof(NEW_DLL_FUNCTIONS, pfnCvarValue2); Count = 4; __asm { JMP CallGameFunction }; }

NEW_DLL_FUNCTIONS MetaDLLNewFuncTable =
{
	pfnOnFreeEntPrivateData,
	pfnGameShutdown,
	pfnShouldCollide,
	pfnCvarValue,
	pfnCvarValue2,
};

const char *FilePathByMemoryPointer(void *Pointer)
{
	MEMORY_BASIC_INFORMATION MBI;
	static char FilePath[MAX_PATH];

	if (!VirtualQuery(Pointer, &MBI, sizeof(MBI)))
		return NULL;

	if (MBI.State != MEM_COMMIT)
		return NULL;

	if (!MBI.AllocationBase)
		return NULL;

	if (!GetModuleFileName((HMODULE)MBI.AllocationBase, FilePath, sizeof(FilePath) - 1))
		return NULL;

	if (!FilePath[0])
		return NULL;

	NormalizePathName(FilePath);
	return FilePath;
}

int PluginIndexByFileName(const char *FilePath)
{
	uint32_t Hash = HM_HashValue(FilePath);

	for (int Index = 0; Index < PluginCount; Index++)
	{
		if (Hash == Plugins[Index].PathHash && !strcmp(FilePath, Plugins[Index].PathName))
			return Index;
	}

	return -1;
}

void pfnCVarRegisterSpecial(cvar_t *Value)
{
	if (HM_Retrieve(CVarHashMap, Value->name, NULL))
		return;

	cvar_t *CurCVar = malloc(sizeof(cvar_t));
	memcpy(CurCVar, Value, sizeof(cvar_t));
	HM_Insert(CVarHashMap, Value->name, CurCVar);
	CVAR_REGISTER(CurCVar);
}

void pfnAddServerCommandHandler()
{
	MetaCmd *Command;
	
	if (!HM_Retrieve(CmdHashMap, CMD_ARGV(0), &Command))
		return;
	
	if (Plugins[Command->PluginIndex].Status == PL_RUNNING && Command->Function)
		Command->Function();
}

void pfnAddServerCommandSpecial(const char *Command, void *Function)
{
	const char *FilePath = FilePathByMemoryPointer(Function);
	
	if (!FilePath)
		return;

	int PluginIndex = PluginIndexByFileName(FilePath);

	if (PluginIndex < 0)
		return;

	if (HM_Retrieve(CmdHashMap, Command, NULL))
		return;

	MetaCmd *CurCommand = malloc(sizeof(MetaCmd));
	CurCommand->PluginIndex = PluginIndex;
	CurCommand->Function = Function;
	HM_Insert(CmdHashMap, Command, CurCommand);
	REG_SVR_COMMAND((char *)Command, pfnAddServerCommandHandler);
}

DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS *_FunctionTable, int *_InterfaceVersion)
{
	if (!_FunctionTable || *_InterfaceVersion != INTERFACE_VERSION)
	{
		*_InterfaceVersion = INTERFACE_VERSION;
		return 0;
	}

	memcpy(_FunctionTable, &MetaDLLFuncTable, sizeof(DLL_FUNCTIONS));
	return TRUE;
}

DLLEXPORT int GetNewDLLFunctions(NEW_DLL_FUNCTIONS *_FunctionTable, int *_InterfaceVersion)
{
	if (!_FunctionTable || *_InterfaceVersion != NEW_DLL_FUNCTIONS_VERSION)
	{
		*_InterfaceVersion = NEW_DLL_FUNCTIONS_VERSION;
		return 0;
	}

	memcpy(_FunctionTable, &MetaDLLNewFuncTable, sizeof(NEW_DLL_FUNCTIONS));
	return TRUE;
}

char GameDir[MAX_PATH];

extern int GameDLL_Init(char *GameDir);
extern int MetaMod_Init(char *GameDir);
extern GAME_DLL_FUNCTIONS MFunctions;

int Initialize(void)
{
	EngineFuncs.pfnGetGameDir(GameDir);
	MGlobals.OrigReturn = &RetVal;
	MGlobals.OverrideReturn = &RetValOver;
	MessageHashMap = calloc(sizeof(char), sizeof(StringHashMap));
	CVarHashMap = calloc(sizeof(char), sizeof(StringHashMap));
	CmdHashMap = calloc(sizeof(char), sizeof(StringHashMap));
	
	if (GameDLL_Init(GameDir))
		return -1;

	MFunctions.DLLFuncs = &GameDLLFuncs;
	MFunctions.NewDLLFuncs = &GameNewDLLFuncs;
	memcpy(&PluginEngineFuncs, &EngineFuncs, sizeof(enginefuncs_t));
	PluginEngineFuncs.pfnCVarRegister = pfnCVarRegisterSpecial;
	PluginEngineFuncs.pfnCvar_RegisterVariable = pfnCVarRegisterSpecial;
	PluginEngineFuncs.pfnAddServerCommand = pfnAddServerCommandSpecial;

	if (MetaMod_Init(GameDir))
		return -1;

	return 0;
}

DLLEXPORT __declspec(naked) void GiveFnptrsToDll(enginefuncs_t *_EngineFuncs, globalvars_t *_GlobalVars)
{
	__asm
	{
		push		ebp
		mov			ebp, esp
		sub			esp, __LOCAL_SIZE
		push		ebx
		push		esi
		push		edi
	}

	memcpy(&EngineFuncs, _EngineFuncs, sizeof(enginefuncs_t));
	GlobalVars = _GlobalVars;
	ColoredPrint_Init();
	
	if (Initialize())
		ServerPrintColored(TYPE_ERROR, "MetaMod Initialization Failed!");

	__asm
	{
		pop	edi
		pop	esi
		pop	ebx
		mov	esp, ebp
		pop	ebp
		ret 8
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	return TRUE;
}