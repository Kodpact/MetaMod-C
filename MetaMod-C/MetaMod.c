#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <Windows.h>

#include "Types.h"
#include "UTIL.h"
#include "MetaFuncs.h"
#include "HashMap.h"

extern DLL_FUNCTIONS GameDLLFuncs;
extern NEW_DLL_FUNCTIONS GameNewDLLFuncs;
extern IMAGE_DOS_HEADER __ImageBase;
extern MetaGlobals MGlobals;
extern Plugin *Plugins;
extern int PluginCount;

GAME_DLL_FUNCTIONS MFunctions;
char Directory[MAX_PATH];
char PathName[MAX_PATH];

extern char GameDir[MAX_PATH];

BOOL FindMetaPlugin(char *PluginPath, uint32_t PluginHash)
{
	for (int Index = 0; Index < PluginCount - 1; Index++)
	{
		if (Plugins[Index].PathHash == PluginHash && !strcmp(Plugins[Index].PathName, PluginPath))
			return TRUE;
	}

	return FALSE;
}

#define CheckAndCallExport(Type, Offset, Name, Dest, DestType, Version)\
	if (PluginFuncs->Offset)\
	{\
		Type Variable = (Type)GetProcAddress(CurPlugin->Handle, Name); \
		\
		if (Variable)\
			((Type)Variable)(Dest, Version); \
		else\
			ServerPrintColored(TYPE_WARNING, "Cannot Find Required Function - "#Name"");\
	}\

void LoadPlugin(char *Path, PLUG_LOADTIME LT)
{
	NormalizePathName(Path);
	Plugins = realloc(Plugins, (PluginCount + 1) * sizeof(Plugin));
	
	if (!Plugins)
	{
	CantAllocMemory:;
		ServerPrintColored(TYPE_ERROR, "FATAL ERROR: Cannot Allocate Memory.");
		return;
	}
	
	Plugin *CurPlugin = &Plugins[PluginCount];
	PluginCount++;
	CurPlugin->Info = malloc(sizeof(PluginInfo));
	
	if (!CurPlugin->Info)
		goto CantAllocMemory;

	META_QUERY_FN FQuery;
	META_INIT_FN FInit;
	META_ATTACH_FN FAttach;
	PFN_GiveFnptrsToDll FGiveFnptrsToDll;

	wsprintf(PathName, "%s/%s/%s", Directory, GameDir, Path);
	NormalizePathName(PathName);
	CurPlugin->PathName = strdup(PathName);
	CurPlugin->PathHash = HM_HashValue(CurPlugin->PathName);
	CurPlugin->FileName = GetDLLFileName(CurPlugin->PathName);
	CurPlugin->Desc = CurPlugin->FileName;
	CurPlugin->Status = PL_FAILED;
	
	if (FindMetaPlugin(CurPlugin->PathName, CurPlugin->PathHash))
	{
		ServerPrintColored(TYPE_WARNING, "Plugin Duplicated ('%s').", CurPlugin->Desc);
		return;
	}

	CurPlugin->Handle = LoadLibrary(CurPlugin->PathName);
	
	if (!CurPlugin->Handle)
	{
		CurPlugin->Status = PL_BADFILE;
		ServerPrintColored(TYPE_WARNING, "Plugin '%s' Failed To Load. (Unable To Load)", CurPlugin->Desc);
		return;
	}

	FQuery = (META_QUERY_FN)GetProcAddress(CurPlugin->Handle, "Meta_Query");

	if (!FQuery)
	{
		ServerPrintColored(TYPE_WARNING, "Plugin '%s' Failed To Load. (Missing Meta_Query).", CurPlugin->Desc);
		return;
	}

	FInit = (META_INIT_FN)GetProcAddress(CurPlugin->Handle, "Meta_Init");

	if (FInit)
		FInit();

	FGiveFnptrsToDll = (PFN_GiveFnptrsToDll)GetProcAddress(CurPlugin->Handle, "GiveFnptrsToDll");

	if (!FGiveFnptrsToDll)
	{
		ServerPrintColored(TYPE_WARNING, "Plugin '%s' Failed To Load. (Missing GiveFnptrsToDll).");
		return;
	}

extern enginefuncs_t PluginEngineFuncs;

	FGiveFnptrsToDll(&PluginEngineFuncs, GlobalVars);

	if (!FQuery(META_INTERFACE_VERSION, &CurPlugin->Info, &MetaFuncs))
	{
		CurPlugin->Status = PL_DENIED;
		ServerPrintColored(TYPE_INFO, "Plugin '%s' Failed To Load. (Potentially Unsupported Version)", CurPlugin->Desc);
		return;
	}

	CurPlugin->Desc = CurPlugin->Info->Name;
	FAttach = (META_ATTACH_FN)GetProcAddress(CurPlugin->Handle, "Meta_Attach");
	
	if (!FAttach)
	{
		CurPlugin->Status = PL_DENIED;
		return ServerPrintColored(TYPE_WARNING, "Plugin '%s' Failed To Load. (Missing Meta_Attach)");
	}

	META_FUNCTIONS *PluginFuncs = calloc(1, sizeof(META_FUNCTIONS));
	GAME_DLL_FUNCTIONS *PluginDLLFuncs = malloc(sizeof(GAME_DLL_FUNCTIONS));

	if (!PluginFuncs || !PluginDLLFuncs)
		goto CantAllocMemory;
	
	PluginDLLFuncs->DLLFuncs = malloc(sizeof(DLL_FUNCTIONS));
	PluginDLLFuncs->NewDLLFuncs = malloc(sizeof(NEW_DLL_FUNCTIONS));
	memcpy(PluginDLLFuncs->DLLFuncs, &GameDLLFuncs, sizeof(DLL_FUNCTIONS));
	memcpy(PluginDLLFuncs->NewDLLFuncs, &GameNewDLLFuncs, sizeof(NEW_DLL_FUNCTIONS));
	
	int PluginIndex = PluginCount - 1;
	int Result = FAttach(LT, PluginFuncs, &MGlobals, PluginDLLFuncs);
	CurPlugin = &Plugins[PluginIndex];
	
	if (!Result)
		return ServerPrintColored(TYPE_WARNING, "Plugin '%s' Failed To Load. (Meta_Attach)", CurPlugin->Desc);

	CurPlugin->PreTables.DLL = calloc(1, sizeof(DLL_FUNCTIONS));
	CurPlugin->PreTables.NewDLL = calloc(1, sizeof(NEW_DLL_FUNCTIONS));
	CurPlugin->PreTables.Engine = calloc(1, sizeof(enginefuncs_t));
	CurPlugin->PostTables.DLL = calloc(1, sizeof(DLL_FUNCTIONS));
	CurPlugin->PostTables.NewDLL = calloc(1, sizeof(NEW_DLL_FUNCTIONS));
	CurPlugin->PostTables.Engine = calloc(1, sizeof(enginefuncs_t));
	
	if (!CurPlugin->PreTables.DLL || !CurPlugin->PreTables.NewDLL || !CurPlugin->PreTables.Engine ||
		!CurPlugin->PostTables.DLL || !CurPlugin->PostTables.NewDLL || !CurPlugin->PostTables.Engine)
		goto CantAllocMemory;

	int IV = INTERFACE_VERSION;
	CheckAndCallExport(GETENTITYAPI_FN, pfnGetEntityAPI, "GetEntityAPI", CurPlugin->PreTables.DLL, DLL_FUNCTIONS, INTERFACE_VERSION);
	CheckAndCallExport(GETENTITYAPI2_FN, pfnGetEntityAPI2, "GetEntityAPI2", CurPlugin->PreTables.DLL, DLL_FUNCTIONS, &IV);
	
	CheckAndCallExport(GETENTITYAPI_FN, pfnGetEntityAPI_Post, "GetEntityAPI_Post", CurPlugin->PostTables.DLL, DLL_FUNCTIONS, INTERFACE_VERSION);
	CheckAndCallExport(GETENTITYAPI2_FN, pfnGetEntityAPI2_Post, "GetEntityAPI2_Post", CurPlugin->PostTables.DLL, DLL_FUNCTIONS, &IV);
	
	IV = NEW_DLL_FUNCTIONS_VERSION;
	CheckAndCallExport(GETNEWDLLFUNCTIONS_FN, pfnGetNewDLLFunctions, "GetNewDLLFunctions", CurPlugin->PreTables.NewDLL, NEW_DLL_FUNCTIONS, &IV);
	CheckAndCallExport(GETNEWDLLFUNCTIONS_FN, pfnGetNewDLLFunctions_Post, "GetNewDLLFunctions_Post", CurPlugin->PostTables.NewDLL, NEW_DLL_FUNCTIONS, &IV);
	
	IV = ENGINE_INTERFACE_VERSION;
	CheckAndCallExport(GET_ENGINE_FUNCTIONS_FN, pfnGetEngineFunctions, "GetEngineFunctions", CurPlugin->PreTables.Engine, enginefuncs_t, &IV);
	CheckAndCallExport(GET_ENGINE_FUNCTIONS_FN, pfnGetEngineFunctions_Post, "GetEngineFunctions_Post", CurPlugin->PostTables.Engine, enginefuncs_t, &IV);
	
	CurPlugin->Status = PL_RUNNING;
	ServerPrintColored(TYPE_INFO, "Plugin '%s' Loaded Successfully.", CurPlugin->Desc);
}

void MetaMod_SetPluginStatus(int Status)
{
	const char *Desc = CMD_ARGV(2);
	int PluginIndex = -1;

	for (int Index = 0; Index < PluginCount; Index++)
	{
		if (!stricmp(Desc, Plugins[Index].Desc))
		{
			PluginIndex = Index;
			break;
		}
	}

	if (PluginIndex < 0)
		return ServerPrintColored(TYPE_ERROR, "Cannot Find Plugin '%s'.", Desc);

	int CurStatus = Plugins[PluginIndex].Status;

	switch (Status)
	{
		case PL_PAUSED:
		{
			if (CurStatus == PL_PAUSED)
				return ServerPrintColored(TYPE_WARNING, "Plugin Already Paused (%s).", Desc);

			Plugins[PluginIndex].Status = PL_PAUSED;
			ServerPrintColored(TYPE_WARNING, "Plugin '%s' Has Been Paused.", Desc);
			break;
		}
		case PL_RUNNING:
		{
			if (CurStatus == PL_RUNNING)
				return ServerPrintColored(TYPE_WARNING, "Plugin Already Running (%s).", Desc);

			Plugins[PluginIndex].Status = PL_RUNNING;
			ServerPrintColored(TYPE_WARNING, "Plugin '%s' Has Been Unpaused.", Desc);
			break;
		}
	}
}

void MetaMod_ShowGPL(void)
{
	ServerPrint("MetaMod-C GPL:\n");
	ServerPrint("     Copyright (c) 2001-2013 Will Day <willday@hpgx.net>, 2018 Ghost\n");
	ServerPrint("     Metamod is free software; you can redistribute it and/or");
	ServerPrint("     modify it under the terms of the GNU General Public License");
	ServerPrint("     as published by the Free Software Foundation; either");
	ServerPrint("     version 2 of the License, or (at your option) any later");
	ServerPrint("     version.\n");
	ServerPrint("     Metamod is distributed in the hope that it will be useful,");
	ServerPrint("     but WITHOUT ANY WARRANTY; without even the implied warranty");
	ServerPrint("     of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");
	ServerPrint("     See the GNU General Public License for more details.\n");
	ServerPrint("     You should have received a copy of the GNU General Public");
	ServerPrint("     License along with Metamod; if not, write to the Free");
	ServerPrint("     Software Foundation, Inc., 59 Temple Place, Suite 330,");
	ServerPrint("     Boston, MA 02111-1307  USA\n");
	ServerPrint("     In addition, as a special exception, the author gives");
	ServerPrint("     permission to link the code of this program with the");
	ServerPrint("     Half-Life Game Engine (\"HL Engine\") and Modified Game");
	ServerPrint("     Libraries (\"MODs\") developed by Valve, L.L.C (\"Valve\").");
	ServerPrint("     You must obey the GNU General Public License in all");
	ServerPrint("     respects for all of the code used other than the HL Engine");
	ServerPrint("     and MODs from Valve.  If you modify this file, you may");
	ServerPrint("     extend this exception to your version of the file, but you");
	ServerPrint("     are not obligated to do so.  If you do not wish to do so,");
	ServerPrint("     delete this exception statement from your version.\n");
}

const char *MetaMod_GetPluginStatus(int Status)
{
	switch (Status)
	{
		case PL_DENIED: return "Failed | Rejected To Load";
		case PL_BADFILE: return "Failed | Bad File";
		case PL_FAILED: return "Failed | Unknown";
		case PL_PAUSED: return "Paused";
		case PL_RUNNING: return "Running";
	}

	return "<Missing>";
}

void MetaMod_ShowList(void)
{
	ServerPrint("MetaMod-C Plugins:");
	int TotalRunning = 0;
	
	for (int Index = 0; Index < PluginCount; Index++)
	{
		Plugin *CurPlugin = &Plugins[Index];
		ServerPrintColored(CurPlugin->Status == PL_RUNNING ? TYPE_INFO : CurPlugin->Status == PL_PAUSED ?
					   TYPE_WARNING : TYPE_ERROR, "     %s (Status: %s)", CurPlugin->Desc ? CurPlugin->Desc : "<Missing>",
									  MetaMod_GetPluginStatus(CurPlugin->Status));

		if (CurPlugin->Status = PL_RUNNING)
			TotalRunning++;
	}

	ServerPrint("Total %i Plugins (%i Running)", PluginCount, TotalRunning);
}

#pragma warning (disable : 4098)

void MetaCmdHandler(void)
{
	if (CMD_ARGC() < 2)
	{
		ServerPrintColored(TYPE_INFO, "Meta Arguments Are:\n     Version - Show MetaMod-C Version\n     GPL - Show General Public License\n     List - Show Plugin List\n     Pause - Pause Plugin By Name\n     Unpause - Unpause Plugin By Name");
		return;
	}

	const char *Argument = CMD_ARGV(1);

	if (!stricmp(Argument, "version"))
		return ServerPrintColored(TYPE_INFO, META_VERSION META_AUTHOR);
	
	if (!stricmp(Argument, "gpl"))
		return MetaMod_ShowGPL();

	if (!stricmp(Argument, "list"))
		return MetaMod_ShowList();
	
	if (!stricmp(Argument, "pause"))
	{
		if (CMD_ARGC() < 3)
			return ServerPrintColored(TYPE_ERROR, "Missing Third Argument For Pause.");

		return MetaMod_SetPluginStatus(PL_PAUSED);
	}

	if (!stricmp(Argument, "unpause"))
	{
		if (CMD_ARGC() < 3)
			return ServerPrintColored(TYPE_ERROR, "Missing Third Argument For Unpause.");

		return MetaMod_SetPluginStatus(PL_RUNNING);
	}

	return ServerPrintColored(TYPE_WARNING, "Invalid Argument '%s'.", Argument);
}

int MetaMod_Init(char *GameDir)
{
	MetaFuncs_Init();
	REG_SVR_COMMAND("meta", MetaCmdHandler);

	GetModuleFileName(NULL, Directory, sizeof(Directory) - 1);
	FileToDirectory(Directory);
	char DllPath[MAX_PATH], PluginInfosPath[MAX_PATH];
	GetModuleFileName((HINSTANCE)&__ImageBase, DllPath, MAX_PATH);
	GetPreviousDirectory(DllPath);
	wsprintf(PluginInfosPath, "%s\\plugins.ini", DllPath);
	FILE *PluginInfosFile = fopen(PluginInfosPath, "rb");
	
	if (!PluginInfosFile)
	{
		GetPreviousDirectory(DllPath);
		wsprintf(PluginInfosPath, "%s\\plugins.ini", DllPath);
		PluginInfosFile = fopen(PluginInfosPath, "rb");

		if (!PluginInfosFile)
			return -1;
	}

	fseek(PluginInfosFile, 0L, SEEK_END);
	long Size = min(ftell(PluginInfosFile), 262144);
	char *Buffer = malloc(Size + 1);

	if (!Buffer)
		return -1;

	rewind(PluginInfosFile);
	fread(Buffer, sizeof(char), Size, PluginInfosFile);
	fclose(PluginInfosFile);
	Buffer[Size] = '\0';
	char *Key, *Value, LastChar;
	int Result;
	char *FileType;

	while (TRUE)
	{
		Result = ReadKeyValue(Buffer, &Key, &Value, &Buffer);

		if (Result < 0)
			return -1;
		else if (Result > 0)
			break;

		LastChar = *Buffer;
		*Buffer++ = 0;
		RemoveQuotes(&Value);

		if (*Key != ';' && *Key != '#')
		{
			if (strcmp(Key, "win32") && strcmp(Key, "mswin"))
			{
				ServerPrintColored(TYPE_WARNING, "Plugin '%s' Has Unsupported Platform. (%s)", GetDLLFileName(Value), Key);
				goto IgnoreLine;
			}

			FileType = GetDLLFileType(Value);

			if (strcmp(FileType, "dll"))
			{
				ServerPrintColored(TYPE_WARNING, "Plugin '%s' Has Unsupported File Type. (%s)", GetDLLFileName(Value), FileType);
				goto IgnoreLine;
			}

			LoadPlugin(Value, PT_STARTUP);
		}

	IgnoreLine:

		if (!LastChar)
			break;

		while (*Buffer != 0 && *Buffer != '\n')
			Buffer++;

		if (*Buffer == 0)
			break;
	}

	fclose(PluginInfosFile);
	return 0;
}