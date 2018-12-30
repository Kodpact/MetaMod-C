#pragma once
#include <stdint.h>

#include "HLSDK.h"

#define NOXREF
#define DLLEXPORT __declspec(dllexport)

#define INTERFACE_VERSION 140
#define NEW_DLL_FUNCTIONS_VERSION 1
#define ENGINE_INTERFACE_VERSION 138
#define META_INTERFACE_VERSION "5:13"

#define META_VERSION "MetaMod-C (1.0)\n"
#define META_AUTHOR "Created On MetaMod Basis\nAuthor: Ghost (Original Author: Will Day)"

#define DEST_DLLFUNC 0
#define DEST_NEWDLLFUNC 1
#define DEST_ENGINE 2

extern enginefuncs_t MetaEngineFuncs;
extern DLL_FUNCTIONS MetaDLLFuncTable;
extern NEW_DLL_FUNCTIONS MetaDLLNewFuncTable;

extern enginefuncs_t EngineFuncs;
extern enginefuncs_t PluginEngineFuncs;
extern globalvars_t *GlobalVars;

extern HMODULE GameDLL;

typedef void(*F)(void);

typedef struct
{
	char *Dir;
	char *Lib;
	char *Name;
	uint32_t Hash;
} GameData;

typedef enum
{
	PT_NEVER = 0,
	PT_STARTUP,			// should only be loaded/unloaded at initial hlds execution
	PT_CHANGELEVEL,		// can be loaded/unloaded between maps
	PT_ANYTIME,			// can be loaded/unloaded at any time
	PT_ANYPAUSE,		// can be loaded/unloaded at any time, and can be "paused" during a map
} PLUG_LOADTIME;

typedef struct
{
	char *IfVersion;
	char *Name;
	char *Version;
	char *Date;
	char *Author;
	char *URL;
	char *LogTag;
	PLUG_LOADTIME Loadable;
	PLUG_LOADTIME Unloadable;
} PluginInfo;

typedef PluginInfo* PluginID;

typedef struct
{
	char *Path;
	uint32_t Hash;
} PluginFile;

typedef struct hudtextparms_s
{
	float x;
	float y;
	int effect;
	byte r1, g1, b1, a1;
	byte r2, g2, b2, a2;
	float fadeinTime;
	float fadeoutTime;
	float holdTime;
	float fxTime;
	int channel;
} hudtextparms_t;

typedef enum
{
	PNL_NULL = 0,
	PNL_INI_DELETED,		// was deleted from plugins.ini
	PNL_FILE_NEWER,			// file on disk is newer than last load
	PNL_COMMAND,			// requested by server/console command
	PNL_CMD_FORCED,			// forced by server/console command
	PNL_DELAYED,			// delayed from previous request; can't tell origin
	//only used for 'real_reason' on MPlugin::unload()
	PNL_PLUGIN,			// requested by plugin function call
	PNL_PLG_FORCED,			// forced by plugin function call
	//only used internally for 'meta reload'
	PNL_RELOAD,			// forced unload by reload()
} PL_UNLOAD_REASON;

typedef enum
{
	GINFO_NAME = 0,
	GINFO_DESC,
	GINFO_GAMEDIR,
	GINFO_DLL_FULLPATH,
	GINFO_DLL_FILENAME,
	GINFO_REALDLL_FULLPATH,
} GameInfoE;

typedef struct
{
	const char *Name;
	const char *Desc;
	const char *GameDir;
	const char *DllFullPath;
	const char *DllFileName;
	const char *RealDllFullPath;
} GameInfo;

typedef struct
{
	void(*pfnLogConsole)		(PluginID, const char *, ...);
	void(*pfnLogMessage)		(PluginID, const char *, ...);
	void(*pfnLogError)			(PluginID, const char *, ...);
	void(*pfnLogDeveloper)		(PluginID, const char *, ...);
	void(*pfnCenterSay)			(PluginID, const char *, ...);
	void(*pfnCenterSayParms)	(PluginID, hudtextparms_t, const char *, ...);
	void(*pfnCenterSayVarargs)	(PluginID, hudtextparms_t, const char *, va_list);
	qboolean(*pfnCallGameEntity)	(PluginID, const char *, entvars_t *);
	int(*pfnGetUserMsgID)		(PluginID, const char *, int *);
	const char *(*pfnGetUserMsgName)	(PluginID, int, int *);
	const char *(*pfnGetPluginPath)		(PluginID);
	const char *(*pfnGetGameInfo)		(PluginID, GameInfo);
	int(*pfnLoadPlugin)(PluginID, const char *, PLUG_LOADTIME , void **);
	int(*pfnUnloadPlugin)(PluginID, const char *, PLUG_LOADTIME , PL_UNLOAD_REASON);
	int(*pfnUnloadPluginByHandle)(PluginID, void *, PLUG_LOADTIME , PL_UNLOAD_REASON);
	const char *(*pfnIsQueryingClientCvar)	(PluginID, const edict_t *);
	int(*pfnMakeRequestID)	(PluginID);
	void(*pfnGetHookTables)             (PluginID, enginefuncs_t **, DLL_FUNCTIONS **, NEW_DLL_FUNCTIONS **);
} META_FUNCS;

typedef int(*GETENTITYAPI_FN) (DLL_FUNCTIONS *, int );
typedef int(*GETENTITYAPI2_FN) (DLL_FUNCTIONS *, int *);
typedef int(*GETNEWDLLFUNCTIONS_FN) (NEW_DLL_FUNCTIONS *, int *);
typedef int(*GET_ENGINE_FUNCTIONS_FN) (enginefuncs_t *, int *);

typedef struct
{
	GETENTITYAPI_FN         pfnGetEntityAPI;
	GETENTITYAPI_FN         pfnGetEntityAPI_Post;
	GETENTITYAPI2_FN        pfnGetEntityAPI2;
	GETENTITYAPI2_FN        pfnGetEntityAPI2_Post;
	GETNEWDLLFUNCTIONS_FN   pfnGetNewDLLFunctions;
	GETNEWDLLFUNCTIONS_FN   pfnGetNewDLLFunctions_Post;
	GET_ENGINE_FUNCTIONS_FN pfnGetEngineFunctions;
	GET_ENGINE_FUNCTIONS_FN pfnGetEngineFunctions_Post;
} META_FUNCTIONS;

typedef enum
{
	MRES_UNSET = 0,
	MRES_IGNORED,		// plugin didn't take any action
	MRES_HANDLED,		// plugin did something, but real function should still be called
	MRES_OVERRIDE,		// call real function, but use my return value
	MRES_SUPERCEDE,		// skip real function; use my return value
} META_RES;

typedef struct MetaGlobals_S
{
	META_RES Result;			// writable; plugin's return flag
	META_RES PrevResult;		// readable; return flag of the previous plugin called
	META_RES Status;			// readable; "highest" return flag so far
	void *OrigReturn;			// readable; return value from "real" function
	void *OverrideReturn;		// readable; return value from overriding/superceding plugin
} MetaGlobals;

typedef struct
{
	DLL_FUNCTIONS *DLLFuncs;
	NEW_DLL_FUNCTIONS *NewDLLFuncs;
} GAME_DLL_FUNCTIONS;

typedef enum
{
	PL_NONE = 0,
	PL_DENIED,
	PL_BADFILE, 
	PL_FAILED,
	PL_RUNNING,
	PL_PAUSED,
} PLUG_STATUS;

// Action to take for plugin at next opportunity.
typedef enum
{
	PA_NULL = 0,
	PA_NONE,			// no action needed right now
	PA_KEEP,			// keep, after ini refresh
	PA_LOAD,			// load (dlopen, query) and try to attach
	PA_ATTACH,			// attach
	PA_UNLOAD,			// unload (detach, dlclose)
	PA_RELOAD,			// unload and load again
} PLUG_ACTION;

// Flags to indicate from where the plugin was loaded.
typedef enum
{
	PS_INI = 0,			// was loaded from the plugins.ini
	PS_CMD,				// was loaded via a server command
	PS_PLUGIN,			// was loaded by other plugin
} PLOAD_SOURCE;

// Flags for how to word description of plugin loadtime.
typedef enum
{
	SL_SIMPLE = 0,			// single word
	SL_SHOW,			// for "show" output, 5 chars
	SL_ALLOWED,			// when plugin is allowed to load/unload
	SL_NOW,				// current situation
} STR_LOADTIME;

// Flags for how to format description of status.
typedef enum
{
	ST_SIMPLE = 0,			// single word
	ST_SHOW,			// for "show" output, 4 chars
} STR_STATUS;

// Flags for how to format description of action.
typedef enum
{
	SA_SIMPLE = 0,			// single word
	SA_SHOW,			// for "show" output, 4 chars
} STR_ACTION;

// Flags for how to format description of source.
typedef enum
{
	SO_SIMPLE = 0,			// two words
	SO_SHOW,			// for "list" output, 3 chars
} STR_SOURCE;

typedef struct
{
	DLL_FUNCTIONS *DLL;
	NEW_DLL_FUNCTIONS *NewDLL;
	enginefuncs_t *Engine;
} HookTables;

typedef struct
{
	PLUG_STATUS Status;
	HookTables PreTables;
	HookTables PostTables;

	int Index;					// 1-based
	PLUG_ACTION action;				// what to do with plugin (load, unload, etc)
	PLOAD_SOURCE source;				// source of the request to load the plugin
	int source_plugin_index;			// index of plugin that loaded this plugin. -1 means source plugin has been unloaded.
	int unloader_index;
	BOOL is_unloader;				// fix to prevent other plugins unload active unloader.

	HINSTANCE Handle;				// handle for dlopen, dlsym, etc
	PluginInfo *Info;				// information plugin provides about itself
	time_t LoadTime;				// when plugin was loaded

	char *FileName;
	char *Desc;
	char *PathName;
	uint32_t PathHash;
} Plugin;

typedef struct
{
	const char *Name;
	int Size;
	int Index;
} MetaMsg;

typedef struct
{
	int PluginIndex;
	F Function;
} MetaCmd;

typedef void(__stdcall *PFN_GiveFnptrsToDll)(enginefuncs_t *, globalvars_t *);
typedef int(*APIFUNCTION2)(DLL_FUNCTIONS *, int *);
typedef int(*APIFUNCTION)(DLL_FUNCTIONS *, int);
typedef int(*NEW_DLL_FUNCTIONS_FN)(NEW_DLL_FUNCTIONS *, int *);

typedef int(*META_QUERY_FN) (char *, PluginInfo **, META_FUNCS *);
typedef void(*META_INIT_FN) (void);
typedef int(*META_ATTACH_FN) (PLUG_LOADTIME Now, META_FUNCTIONS *, MetaGlobals *, GAME_DLL_FUNCTIONS *);
typedef void(*ENTITY_FN) (entvars_t *);

extern META_FUNCS MetaFuncs;
extern void LoadPlugin(char *PluginPath, PLUG_LOADTIME LT);