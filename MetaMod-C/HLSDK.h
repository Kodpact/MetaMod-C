#pragma once
#include <stdint.h>
#include <Windows.h>

#define MAX_ENT_LEAFS 48
#define MAX_LEVEL_CONNECTIONS 16
#define MAX_QPATH 64 
#define NEW_DLL_FUNCTIONS_VERSION 1
#define MAX_USER_MESSAGES 256 - 64

#define	FCVAR_ARCHIVE		(1<<0)	// set to cause it to be saved to vars.rc
#define	FCVAR_USERINFO		(1<<1)	// changes the client's info string
#define	FCVAR_SERVER		(1<<2)	// notifies players when changed
#define FCVAR_EXTDLL		(1<<3)	// defined by external DLL
#define FCVAR_CLIENTDLL     (1<<4)  // defined by the client dll
#define FCVAR_PROTECTED     (1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY        (1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define FCVAR_PRINTABLEONLY (1<<7)  // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define FCVAR_UNLOGGED		(1<<8)  // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NOEXTRAWHITEPACE	(1<<9)  // strip trailing/leading white space from this cvar

#define MESSAGE_BEGIN		(EngineFuncs.pfnMessageBegin)
#define MESSAGE_END			(EngineFuncs.pfnMessageEnd)
#define WRITE_BYTE			(EngineFuncs.pfnWriteByte)
#define WRITE_CHAR			(EngineFuncs.pfnWriteChar)
#define WRITE_SHORT			(EngineFuncs.pfnWriteShort)
#define WRITE_LONG			(EngineFuncs.pfnWriteLong)
#define WRITE_ANGLE			(EngineFuncs.pfnWriteAngle)
#define WRITE_COORD			(EngineFuncs.pfnWriteCoord)
#define WRITE_STRING			(EngineFuncs.pfnWriteString)
#define WRITE_ENTITY			(EngineFuncs.pfnWriteEntity)
#define CVAR_REGISTER			(EngineFuncs.pfnCVarRegister)
#define CVAR_GET_FLOAT			(EngineFuncs.pfnCVarGetFloat)
#define CVAR_GET_STRING			(EngineFuncs.pfnCVarGetString)
#define CVAR_SET_FLOAT			(EngineFuncs.pfnCVarSetFloat)
#define CVAR_SET_STRING			(EngineFuncs.pfnCVarSetString)
#define CVAR_GET_POINTER		(EngineFuncs.pfnCVarGetPointer)
#define ALERT				(EngineFuncs.pfnAlertMessage)
#define ENGINE_FSERVER_PRINT			(EngineFuncs.pfnEngineFprintf)
#define ALLOC_PRIVATE			(EngineFuncs.pfnPvAllocEntPrivateData)
#define FREE_PRIVATE			(EngineFuncs.pfnFreeEntPrivateData)
//#define STRING			(EngineFuncs.pfnSzFromIndex)
#define ALLOC_STRING			(EngineFuncs.pfnAllocString)
#define FIND_ENTITY_BY_STRING		(EngineFuncs.pfnFindEntityByString)
#define GETENTITYILLUM			(EngineFuncs.pfnGetEntityIllum)
#define FIND_ENTITY_IN_SPHERE		(EngineFuncs.pfnFindEntityInSphere)
#define FIND_CLIENT_IN_PVS		(EngineFuncs.pfnFindClientInPVS)
#define FIND_ENTITY_IN_PVS		(EngineFuncs.pfnEntitiesInPVS)
#define EMIT_AMBIENT_SOUND		(EngineFuncs.pfnEmitAmbientSound)
#define GET_MODEL_PTR			(EngineFuncs.pfnGetModelPtr)
#define REG_USER_MSG			(EngineFuncs.pfnRegUserMsg)
#define GET_BONE_POSITION		(EngineFuncs.pfnGetBonePosition)
#define FUNCTION_FROM_NAME		(EngineFuncs.pfnFunctionFromName)
#define NAME_FOR_FUNCTION		(EngineFuncs.pfnNameForFunction)
#define TRACE_TEXTURE			(EngineFuncs.pfnTraceTexture)
#define CLIENT_SERVER_PRINT			(EngineFuncs.pfnClientSERVER_PRINT)
#define SERVER_PRINT			(EngineFuncs.pfnServerPrint)
#define CMD_ARGS			(EngineFuncs.pfnCmd_Args)
#define CMD_ARGC			(EngineFuncs.pfnCmd_Argc)
#define CMD_ARGV			(EngineFuncs.pfnCmd_Argv)
#define GET_ATTACHMENT			(EngineFuncs.pfnGetAttachment)
#define SET_VIEW			(EngineFuncs.pfnSetView)
#define SET_CROSSHAIRANGLE		(EngineFuncs.pfnCrosshairAngle)
#define LOAD_FILE_FOR_ME		(EngineFuncs.pfnLoadFileForMe)
#define FREE_FILE			(EngineFuncs.pfnFreeFile)
#define END_SECTION			(EngineFuncs.pfnEndSection)
#define COMPARE_FILE_TIME		(EngineFuncs.pfnCompareFileTime)
#define GET_GAME_DIR			(EngineFuncs.pfnGetGameDir)
#define SET_CLIENT_MAXSPEED		(EngineFuncs.pfnSetClientMaxspeed)
#define CREATE_FAKE_CLIENT		(EngineFuncs.pfnCreateFakeClient)
#define PLAYER_RUN_MOVE			(EngineFuncs.pfnRunPlayerMove)
#define NUMBER_OF_ENTITIES		(EngineFuncs.pfnNumberOfEntities)
#define GET_INFO_BUFFER			(EngineFuncs.pfnGetInfoKeyBuffer)
#define GET_KEY_VALUE			(EngineFuncs.pfnInfoKeyValue)
#define SET_KEY_VALUE			(EngineFuncs.pfnSetKeyValue)
#define SET_CLIENT_KEY_VALUE		(EngineFuncs.pfnSetClientKeyValue)
#define IS_MAP_VALID			(EngineFuncs.pfnIsMapValid)
#define STATIC_DECAL			(EngineFuncs.pfnStaticDecal)
#define IS_DEDICATED_SERVER		(EngineFuncs.pfnIsDedicatedServer)
#define PRECACHE_EVENT			(EngineFuncs.pfnPrecacheEvent)
#define PLAYBACK_EVENT_FULL		(EngineFuncs.pfnPlaybackEvent)
#define ENGINE_SET_PVS			(EngineFuncs.pfnSetFatPVS)
#define ENGINE_SET_PAS			(EngineFuncs.pfnSetFatPAS)
#define ENGINE_CHECK_VISIBILITY		(EngineFuncs.pfnCheckVisibility)
#define DELTA_SET			(EngineFuncs.pfnDeltaSetField)
#define DELTA_UNSET			(EngineFuncs.pfnDeltaUnsetField)
#define DELTA_ADDENCODER		(EngineFuncs.pfnDeltaAddEncoder)
#define ENGINE_CURRENT_PLAYER		(EngineFuncs.pfnGetCurrentPlayer)
#define ENGINE_CANSKIP			(EngineFuncs.pfnCanSkipPlayer)
#define DELTA_FINDFIELD			(EngineFuncs.pfnDeltaFindField)
#define DELTA_SETBYINDEX		(EngineFuncs.pfnDeltaSetFieldByIndex)
#define DELTA_UNSETBYINDEX		(EngineFuncs.pfnDeltaUnsetFieldByIndex)
#define REMOVE_KEY_VALUE		(EngineFuncs.pfnInfo_RemoveKey)
#define SET_PHYSICS_KEY_VALUE		(EngineFuncs.pfnSetPhysicsKeyValue)
#define ENGINE_GETPHYSINFO		(EngineFuncs.pfnGetPhysicsInfoString)
#define ENGINE_SETGROUPMASK		(EngineFuncs.pfnSetGroupMask)
#define ENGINE_INSTANCE_BASELINE	(EngineFuncs.pfnCreateInstancedBaseline)
#define ENGINE_FORCE_UNMODIFIED		(EngineFuncs.pfnForceUnmodified)
#define PLAYER_CNX_STATS		(EngineFuncs.pfnGetPlayerStats)
#define GET_INFOKEYBUFFER	    	(EngineFuncs.pfnGetInfoKeyBuffer)
#define INFOKEY_VALUE			    (EngineFuncs.pfnInfoKeyValue)
#define SET_CLIENT_KEYVALUE	    	(EngineFuncs.pfnSetClientKeyValue)
#define REG_SVR_COMMAND	    		(EngineFuncs.pfnAddServerCommand)
#define SERVER_PRINT			    (EngineFuncs.pfnServerPrint)
#define SET_SERVER_KEYVALUE	    	(EngineFuncs.pfnSetKeyValue)
#define QUERY_CLIENT_CVAR_VALUE	    (EngineFuncs.pfnQueryClientCvarValue)
#define QUERY_CLIENT_CVAR_VALUE2	(EngineFuncs.pfnQueryClientCvarValue2)

typedef BOOL qboolean;
typedef uint32_t string_t;
typedef float vec3_t[3];
typedef struct link_s { struct link_s *Previous, *Next; } link_t;
typedef uint32_t CRC32_t;

typedef struct
{
	float time;
	float frametime;
	float force_retouch;
	string_t mapname;
	string_t startspot;
	float deathmatch;
	float coop;
	float teamplay;
	float serverflags;
	float found_secrets;
	vec3_t v_forward;
	vec3_t v_up;
	vec3_t v_right;
	float trace_allsolid;
	float trace_startsolid;
	float trace_fraction;
	vec3_t trace_endpos;
	vec3_t trace_plane_normal;
	float trace_plane_dist;
	struct edict_t *trace_ent;
	float trace_inopen;
	float trace_inwater;
	int trace_hitgroup;
	int trace_flags;
	int msg_entity;
	int cdAudioTrack;
	int maxClients;
	int maxEntities;
	const char *pStringBase;

	void *pSaveData;
	vec3_t vecLandmarkOffset;
} globalvars_t;

typedef struct entvars_s
{
	string_t classname;
	string_t globalname;

	vec3_t origin;
	vec3_t oldorigin;
	vec3_t velocity;
	vec3_t basevelocity;
	vec3_t clbasevelocity;
	vec3_t movedir;

	vec3_t angles;
	vec3_t avelocity;
	vec3_t punchangle;
	vec3_t v_angle;

	vec3_t endpos;
	vec3_t startpos;
	float impacttime;
	float starttime;

	int fixangle;
	float idealpitch;
	float pitch_speed;
	float ideal_yaw;
	float yaw_speed;

	int modelindex;
	string_t model;

	int viewmodel;
	int weaponmodel;

	vec3_t absmin;
	vec3_t absmax;
	vec3_t mins;
	vec3_t maxs;
	vec3_t size;

	float ltime;
	float nextthink;

	int movetype;
	int solid;

	int skin;
	int body;
	int effects;

	float gravity;
	float friction;

	int light_level;

	int sequence;
	int gaitsequence;
	float frame;
	float animtime;
	float framerate;
	byte controller[4];
	byte blending[2];

	float scale;

	int rendermode;
	float renderamt;
	vec3_t rendercolor;
	int renderfx;

	float health;
	float frags;
	int weapons;
	float takedamage;

	int deadflag;
	vec3_t view_ofs;

	int button;
	int impulse;

	struct edict_t *chain;
	struct edict_t *dmg_inflictor;
	struct edict_t *enemy;
	struct edict_t *aiment;
	struct edict_t *owner;
	struct edict_t *groundentity;

	int spawnflags;
	int flags;

	int colormap;
	int team;

	float max_health;
	float teleport_time;
	float armortype;
	float armorvalue;
	int waterlevel;
	int watertype;

	string_t target;
	string_t targetname;
	string_t netname;
	string_t message;

	float dmg_take;
	float dmg_save;
	float dmg;
	float dmgtime;

	string_t noise;
	string_t noise1;
	string_t noise2;
	string_t noise3;

	float speed;
	float air_finished;
	float pain_finished;
	float radsuit_finished;

	struct edict_t *pContainingEntity;

	int playerclass;
	float maxspeed;

	float fov;
	int weaponanim;

	int pushmsec;

	int bInDuck;
	int flTimeStepSound;
	int flSwimTime;
	int flDuckTime;
	int iStepLeft;
	float flFallVelocity;

	int gamestate;

	int oldbuttons;

	int groupinfo;

	int iuser1;
	int iuser2;
	int iuser3;
	int iuser4;
	float fuser1;
	float fuser2;
	float fuser3;
	float fuser4;
	vec3_t vuser1;
	vec3_t vuser2;
	vec3_t vuser3;
	vec3_t vuser4;
	struct edict_t *euser1;
	struct edict_t *euser2;
	struct edict_t *euser3;
	struct edict_t *euser4;
} entvars_t;

typedef struct edict_s
{
	qboolean free;
	int serialnumber;
	link_t area;
	int headnode;
	int num_leafs;
	short leafnums[MAX_ENT_LEAFS];
	float freetime;
	void *pvPrivateData;
	entvars_t v;
} edict_t;

typedef struct ENTITYTABLE_s
{
	int id;
	edict_t *pent;
	int location;
	int size;
	int flags;
	string_t classname;

} ENTITYTABLE;

typedef struct KeyValueData_s
{
	char	*szClassName;
	char	*szKeyName;
	char	*szValue;
	int32_t	fHandled;
} KeyValueData;

typedef struct LEVELLIST_s
{
	char		mapName[32];
	char		landmarkName[32];
	edict_t	*pentLandmark;
	vec3_t		vecLandmarkOrigin;
} LEVELLIST;

typedef enum _fieldtypes
{
	FIELD_FLOAT = 0,
	FIELD_STRING,
	FIELD_ENTITY,
	FIELD_CLASSPTR,
	FIELD_EHANDLE,
	FIELD_EVARS,
	FIELD_EDICT,
	FIELD_VECTOR,
	FIELD_POSITION_VECTOR,
	FIELD_POINTER,
	FIELD_INTEGER,
	FIELD_FUNCTION,
	FIELD_BOOLEAN,
	FIELD_SHORT,
	FIELD_CHARACTER,
	FIELD_TIME,
	FIELD_MODELNAME,
	FIELD_SOUNDNAME,
	FIELD_TYPECOUNT,
} FIELDTYPE;

typedef struct TYPEDESCRIPTION_s
{
	FIELDTYPE fieldType;
	char *fieldName;
	int fieldOffset;
	short fieldSize;
	short flags;
} TYPEDESCRIPTION;

typedef struct saverestore_s
{
	char *pBaseData;
	char *pCurrentData;
	int size;
	int bufferSize;
	int tokenSize;
	int tokenCount;
	char **pTokens;
	int currentIndex;
	int tableCount;
	int connectionCount;
	ENTITYTABLE *pTable;
	LEVELLIST levelList[MAX_LEVEL_CONNECTIONS];

	int fUseLandmark;
	char szLandmarkName[20];
	vec3_t vecLandmarkOffset;
	float time;
	char szCurrentMapName[32];
} SAVERESTOREDATA;

typedef enum
{
	t_sound = 0,
	t_skin,
	t_model,
	t_decal,
	t_generic,
	t_eventscript,
	t_world,
} resourcetype_t;

typedef struct resource_s
{
	char szFileName[MAX_QPATH];
	resourcetype_t type;
	int  nIndex;
	int  nDownloadSize;
	unsigned char ucFlags;

	unsigned char rgucMD5_hash[16];
	unsigned char playernum;

	unsigned char rguc_reserved[32];
	struct resource_s *pNext;
	struct resource_s *pPrev;
} resource_t;

typedef struct customization_s
{
	qboolean bInUse;
	resource_t resource;
	qboolean bTranslated;
	int nUserData1;
	int  nUserData2;
	void *pInfo;
	void *pBuffer;
	struct customization_s *pNext;
} customization_t;

typedef struct DLL_FUNCTIONS_s
{
	void(*pfnGameInit) (void);
	int(*pfnSpawn) (edict_t *pent);
	void(*pfnThink) (edict_t *pent);
	void(*pfnUse) (edict_t *pentUsed, edict_t *pentOther);
	void(*pfnTouch) (edict_t *pentTouched, edict_t *pentOther);
	void(*pfnBlocked) (edict_t *pentBlocked, edict_t *pentOther);
	void(*pfnKeyValue) (edict_t *pentKeyvalue, KeyValueData *pkvd);
	void(*pfnSave) (edict_t *pent, SAVERESTOREDATA *pSaveData);
	int(*pfnRestore) (edict_t *pent, SAVERESTOREDATA *pSaveData, int globalEntity);
	void(*pfnSetAbsBox) (edict_t *pent);
	void(*pfnSaveWriteFields) (SAVERESTOREDATA *, const char *, void *, TYPEDESCRIPTION *, int);
	void(*pfnSaveReadFields) (SAVERESTOREDATA *, const char *, void *, TYPEDESCRIPTION *, int);
	void(*pfnSaveGlobalState) (SAVERESTOREDATA *);
	void(*pfnRestoreGlobalState) (SAVERESTOREDATA *);
	void(*pfnResetGlobalState) (void);
	qboolean(*pfnClientConnect) (edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[128]);
	void(*pfnClientDisconnect) (edict_t *pEntity);
	void(*pfnClientKill) (edict_t *pEntity);
	void(*pfnClientPutInServer) (edict_t *pEntity);
	void(*pfnClientCommand) (edict_t *pEntity);
	void(*pfnClientUserInfoChanged)(edict_t *pEntity, char *infobuffer);
	void(*pfnServerActivate) (edict_t *pedict_tList, int edictCount, int clientMax);
	void(*pfnServerDeactivate) (void);
	void(*pfnPlayerPreThink) (edict_t *pEntity);
	void(*pfnPlayerPostThink) (edict_t *pEntity);
	void(*pfnStartFrame) (void);
	void(*pfnParmsNewLevel) (void);
	void(*pfnParmsChangeLevel) (void);
	const char *(*pfnGetGameDescription)(void);
	void(*pfnPlayerCustomization) (edict_t *pEntity, customization_t *pCustom);
	void(*pfnSpectatorConnect) (edict_t *pEntity);
	void(*pfnSpectatorDisconnect) (edict_t *pEntity);
	void(*pfnSpectatorThink) (edict_t *pEntity);
	void(*pfnSys_Error) (const char *error_string);
	void(*pfnPM_Move) (struct playermove_s *ppmove, qboolean server);
	void(*pfnPM_Init) (struct playermove_s *ppmove);
	char(*pfnPM_FindTextureType)(const char *name);
	void(*pfnSetupVisibility)(struct edict_s *pViewEntity, struct edict_s *pClient, unsigned char **pvs, unsigned char **pas);
	void(*pfnUpdateClientData) (const struct edict_s *ent, int sendweapons, struct clientdata_s *cd);
	int(*pfnAddToFullPack)(struct entity_state_s *state, int e, edict_t *ent, edict_t *host, int hostflags, int player, unsigned char *pSet);
	void(*pfnCreateBaseline) (int player, int eindex, struct entity_state_s *baseline, struct edict_s *entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs);
	void(*pfnRegisterEncoders) (void);
	int(*pfnGetWeaponData) (struct edict_s *player, struct weapon_data_s *info);
	void(*pfnCmdStart) (const edict_t *player, const struct usercmd_s *cmd, unsigned int random_seed);
	void(*pfnCmdEnd) (const edict_t *player);
	int(*pfnConnectionlessPacket) (const struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size);
	int(*pfnGetHullBounds) (int hullnumber, float *mins, float *maxs);
	void(*pfnCreateInstancedBaselines) (void);
	int(*pfnInconsistentFile)(const struct edict_s *player, const char *filename, char *disconnect_message);
	int(*pfnAllowLagCompensation)(void);
} DLL_FUNCTIONS;

typedef struct NEW_DLL_FUNCTIONS_s
{
	void(*pfnOnFreeEntPrivateData)(edict_t *pEnt);
	void(*pfnGameShutdown)(void);
	int(*pfnShouldCollide)(edict_t *pentTouched, edict_t *pentOther);
	void(*pfnCvarValue)(const edict_t *pEnt, const char *value);
	void(*pfnCvarValue2)(const edict_t *pEnt, int requestID, const char *cvarName, const char *value);
} NEW_DLL_FUNCTIONS;

typedef enum
{
	at_notice,
	at_console,
	at_aiconsole,
	at_warning,
	at_error,
	at_logged
} ALERT_TYPE;

typedef enum
{
	print_console,
	print_center,
	print_chat,
} PRINT_TYPE;

typedef struct
{
	int		fAllSolid;
	int		fStartSolid;
	int		fInOpen;
	int		fInWater;
	float	flFraction;
	vec3_t	vecEndPos;
	float	flPlaneDist;
	vec3_t	vecPlaneNormal;
	edict_t	*pHit;
	int		iHitgroup;
} TraceResult;

typedef struct cvar_s
{
	char	*name;
	char	*string;
	int		flags;
	float	value;
	struct cvar_s *next;
} cvar_t;

typedef enum
{
	force_exactfile,
	force_model_samebounds,
	force_model_specifybounds,
	force_model_specifybounds_if_avail,
} FORCE_TYPE;

typedef struct sentenceEntry_ sentenceEntry_s;
struct sentenceEntry_
{
	char* data;
	sentenceEntry_s*		nextEntry;
	qboolean				isGlobal;
	unsigned int			index;
};

typedef struct client_textmessage_s
{
	int		effect;
	byte	r1, g1, b1, a1;
	byte	r2, g2, b2, a2;
	float	x;
	float	y;
	float	fadein;
	float	fadeout;
	float	holdtime;
	float	fxtime;
	const char *pName;
	const char *pMessage;
} client_textmessage_t;

typedef struct sequenceCommandLine_ sequenceCommandLine_s;
typedef struct sequenceCommandLine_
{
	int commandType;
	client_textmessage_t clientMessage;
	char* speakerName;
	char* listenerName;
	char* soundFileName;
	char* sentenceName;
	char* fireTargetNames;
	char* killTargetNames;
	float delay;
	int repeatCount;
	int textChannel;
	int modifierBitField;
	sequenceCommandLine_s*	nextCommandLine;
};

typedef struct sequenceEntry_ sequenceEntry_s;
typedef struct sequenceEntry_
{
	char* fileName;
	char* entryName;
	sequenceCommandLine_s*	firstCommand;
	sequenceEntry_s*		nextEntry;
	qboolean				isGlobal;
};

typedef struct enginefuncs_s
{
	int(*pfnPrecacheModel)			(char* s);
	int(*pfnPrecacheSound)			(char* s);
	void(*pfnSetModel)				(edict_t *e, const char *m);
	int(*pfnModelIndex)			(const char *m);
	int(*pfnModelFrames)			(int modelIndex);
	void(*pfnSetSize)				(edict_t *e, const float *rgflMin, const float *rgflMax);
	void(*pfnChangeLevel)			(char* s1, char* s2);
	void(*pfnGetSpawnParms)			(edict_t *ent);
	void(*pfnSaveSpawnParms)		(edict_t *ent);
	float(*pfnVecToYaw)				(const float *rgflVector);
	void(*pfnVecToAngles)			(const float *rgflVectorIn, float *rgflVectorOut);
	void(*pfnMoveToOrigin)			(edict_t *ent, const float *pflGoal, float dist, int iMoveType);
	void(*pfnChangeYaw)				(edict_t* ent);
	void(*pfnChangePitch)			(edict_t* ent);
	edict_t*	(*pfnFindEntityByString)	(edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue);
	int(*pfnGetEntityIllum)		(edict_t* pEnt);
	edict_t*	(*pfnFindEntityInSphere)	(edict_t *pEdictStartSearchAfter, const float *org, float rad);
	edict_t*	(*pfnFindClientInPVS)		(edict_t *pEdict);
	edict_t* (*pfnEntitiesInPVS)			(edict_t *pplayer);
	void(*pfnMakeVectors)			(const float *rgflVector);
	void(*pfnAngleVectors)			(const float *rgflVector, float *forward, float *right, float *up);
	edict_t*	(*pfnCreateEntity)			(void);
	void(*pfnRemoveEntity)			(edict_t* e);
	edict_t*	(*pfnCreateNamedEntity)		(int className);
	void(*pfnMakeStatic)			(edict_t *ent);
	int(*pfnEntIsOnFloor)			(edict_t *e);
	int(*pfnDropToFloor)			(edict_t* e);
	int(*pfnWalkMove)				(edict_t *ent, float yaw, float dist, int iMode);
	void(*pfnSetOrigin)				(edict_t *e, const float *rgflOrigin);
	void(*pfnEmitSound)				(edict_t *entity, int channel, const char *sample, /*int*/float volume, float attenuation, int fFlags, int pitch);
	void(*pfnEmitAmbientSound)		(edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch);
	void(*pfnTraceLine)				(const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
	void(*pfnTraceToss)				(edict_t* pent, edict_t* pentToIgnore, TraceResult *ptr);
	int(*pfnTraceMonsterHull)		(edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr);
	void(*pfnTraceHull)				(const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr);
	void(*pfnTraceModel)			(const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr);
	const char *(*pfnTraceTexture)			(edict_t *pTextureEntity, const float *v1, const float *v2);
	void(*pfnTraceSphere)			(const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr);
	void(*pfnGetAimVector)			(edict_t* ent, float speed, float *rgflReturn);
	void(*pfnServerCommand)			(char* str);
	void(*pfnServerExecute)			(void);
	void(*pfnClientCommand)			(edict_t* pEdict, char* szFmt, ...);
	void(*pfnParticleEffect)		(const float *org, const float *dir, float color, float count);
	void(*pfnLightStyle)			(int style, char* val);
	int(*pfnDecalIndex)			(const char *name);
	int(*pfnPointContents)			(const float *rgflVector);
	void(*pfnMessageBegin)			(int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
	void(*pfnMessageEnd)			(void);
	void(*pfnWriteByte)				(int iValue);
	void(*pfnWriteChar)				(int iValue);
	void(*pfnWriteShort)			(int iValue);
	void(*pfnWriteLong)				(int iValue);
	void(*pfnWriteAngle)			(float flValue);
	void(*pfnWriteCoord)			(float flValue);
	void(*pfnWriteString)			(const char *sz);
	void(*pfnWriteEntity)			(int iValue);
	void(*pfnCVarRegister)			(cvar_t *pCvar);
	float(*pfnCVarGetFloat)			(const char *szVarName);
	const char*	(*pfnCVarGetString)			(const char *szVarName);
	void(*pfnCVarSetFloat)			(const char *szVarName, float flValue);
	void(*pfnCVarSetString)			(const char *szVarName, const char *szValue);
	void(*pfnAlertMessage)			(ALERT_TYPE atype, char *szFmt, ...);
	void(*pfnEngineFprintf)			(void *pfile, char *szFmt, ...);
	void*		(*pfnPvAllocEntPrivateData)	(edict_t *pEdict, int32_t cb);
	void*		(*pfnPvEntPrivateData)		(edict_t *pEdict);
	void(*pfnFreeEntPrivateData)	(edict_t *pEdict);
	const char*	(*pfnSzFromIndex)			(int iString);
	int(*pfnAllocString)			(const char *szValue);
	struct entvars_s*	(*pfnGetVarsOfEnt)			(edict_t *pEdict);
	edict_t*	(*pfnPEntityOfEntOffset)	(int iEntOffset);
	int(*pfnEntOffsetOfPEntity)	(const edict_t *pEdict);
	int(*pfnIndexOfEdict)			(const edict_t *pEdict);
	edict_t*	(*pfnPEntityOfEntIndex)		(int iEntIndex);
	edict_t*	(*pfnFindEntityByVars)		(struct entvars_s* pvars);
	void*		(*pfnGetModelPtr)			(edict_t* pEdict);
	int(*pfnRegUserMsg)			(const char *pszName, int iSize);
	void(*pfnAnimationAutomove)		(const edict_t* pEdict, float flTime);
	void(*pfnGetBonePosition)		(const edict_t* pEdict, int iBone, float *rgflOrigin, float *rgflAngles);
	uint32_t(*pfnFunctionFromName)	(const char *pName);
	const char *(*pfnNameForFunction)		(uint32_t function);
	void(*pfnClientSERVER_PRINT)			(edict_t* pEdict, PRINT_TYPE ptype, const char *szMsg);
	void(*pfnServerPrint)			(const char *szMsg);
	const char *(*pfnCmd_Args)				(void);
	const char *(*pfnCmd_Argv)				(int argc);
	int(*pfnCmd_Argc)				(void);
	void(*pfnGetAttachment)			(const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles);
	void(*pfnCRC32_Init)			(CRC32_t *pulCRC);
	void(*pfnCRC32_ProcessBuffer)   (CRC32_t *pulCRC, void *p, int len);
	void(*pfnCRC32_ProcessByte)     (CRC32_t *pulCRC, unsigned char ch);
	CRC32_t(*pfnCRC32_Final)			(CRC32_t pulCRC);
	int32_t(*pfnRandomLong)			(int32_t  lLow, int32_t  lHigh);
	float(*pfnRandomFloat)			(float flLow, float flHigh);
	void(*pfnSetView)				(const edict_t *pClient, const edict_t *pViewent);
	float(*pfnTime) (void);
	void(*pfnCrosshairAngle)		(const edict_t *pClient, float pitch, float yaw);
	byte *      (*pfnLoadFileForMe)         (char *filename, int *pLength);
	void(*pfnFreeFile)              (void *buffer);
	void(*pfnEndSection)            (const char *pszSectionName);
	int(*pfnCompareFileTime)       (char *filename1, char *filename2, int *iCompare);
	void(*pfnGetGameDir)            (char *szGetGameDir);
	void(*pfnCvar_RegisterVariable) (cvar_t *variable);
	void(*pfnFadeClientVolume)      (const edict_t *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds);
	void(*pfnSetClientMaxspeed)     (const edict_t *pEdict, float fNewMaxspeed);
	edict_t *	(*pfnCreateFakeClient)		(const char *netname);
	void(*pfnRunPlayerMove)			(edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec);
	int(*pfnNumberOfEntities)		(void);
	char*		(*pfnGetInfoKeyBuffer)		(edict_t *e);
	char*		(*pfnInfoKeyValue)			(char *infobuffer, char *key);
	void(*pfnSetKeyValue)			(char *infobuffer, char *key, char *value);
	void(*pfnSetClientKeyValue)		(int clientIndex, char *infobuffer, char *key, char *value);
	int(*pfnIsMapValid)			(char *filename);
	void(*pfnStaticDecal)			(const float *origin, int decalIndex, int entityIndex, int modelIndex);
	int(*pfnPrecacheGeneric)		(char* s);
	int(*pfnGetPlayerUserId)		(edict_t *e);
	void(*pfnBuildSoundMsg)			(edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed);
	int(*pfnIsDedicatedServer)		(void);
	cvar_t		*(*pfnCVarGetPointer)		(const char *szVarName);
	unsigned int(*pfnGetPlayerWONId)		(edict_t *e);
	void(*pfnInfo_RemoveKey)		(char *s, const char *key);
	const char *(*pfnGetPhysicsKeyValue)	(const edict_t *pClient, const char *key);
	void(*pfnSetPhysicsKeyValue)	(const edict_t *pClient, const char *key, const char *value);
	const char *(*pfnGetPhysicsInfoString)	(const edict_t *pClient);
	unsigned short(*pfnPrecacheEvent)		(int type, const char*psz);
	void(*pfnPlaybackEvent)			(int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2);
	unsigned char *(*pfnSetFatPVS)			(float *org);
	unsigned char *(*pfnSetFatPAS)			(float *org);
	int(*pfnCheckVisibility)		(const edict_t *entity, unsigned char *pset);
	void(*pfnDeltaSetField)			(struct delta_s *pFields, const char *fieldname);
	void(*pfnDeltaUnsetField)		(struct delta_s *pFields, const char *fieldname);
	void(*pfnDeltaAddEncoder)		(char *name, void(*conditionalencode)(struct delta_s *pFields, const unsigned char *from, const unsigned char *to));
	int(*pfnGetCurrentPlayer)		(void);
	int(*pfnCanSkipPlayer)			(const edict_t *player);
	int(*pfnDeltaFindField)		(struct delta_s *pFields, const char *fieldname);
	void(*pfnDeltaSetFieldByIndex)	(struct delta_s *pFields, int fieldNumber);
	void(*pfnDeltaUnsetFieldByIndex)(struct delta_s *pFields, int fieldNumber);
	void(*pfnSetGroupMask)			(int mask, int op);
	int(*pfnCreateInstancedBaseline) (int classname, struct entity_state_s *baseline);
	void(*pfnCvar_DirectSet)		(struct cvar_s *var, char *value);
	void(*pfnForceUnmodified)		(FORCE_TYPE type, float *mins, float *maxs, const char *filename);
	void(*pfnGetPlayerStats)		(const edict_t *pClient, int *ping, int *packet_loss);
	void(*pfnAddServerCommand)		(char *cmd_name, void(*function) (void));
	qboolean(*pfnVoice_GetClientListening)(int iReceiver, int iSender);
	qboolean(*pfnVoice_SetClientListening)(int iReceiver, int iSender, qboolean bListen);
	const char *(*pfnGetPlayerAuthId)		(edict_t *e);
	sequenceEntry_s*	(*pfnSequenceGet)			(const char* fileName, const char* entryName);
	sentenceEntry_s*	(*pfnSequencePickSentence)	(const char* groupName, int pickMethod, int *picked);
	int(*pfnGetFileSize)			(char *filename);
	unsigned int(*pfnGetApproxWavePlayLen) (const char *filepath);
	int(*pfnIsCareerMatch)			(void);
	int(*pfnGetLocalizedStringLength)(const char *label);
	void(*pfnRegisterTutorMessageShown)(int mid);
	int(*pfnGetTimesTutorMessageShown)(int mid);
	void(*pfnProcessTutorMessageDecayBuffer)(int *buffer, int bufferLength);
	void(*pfnConstructTutorMessageDecayBuffer)(int *buffer, int bufferLength);
	void(*pfnResetTutorMessageDecayData)(void);
	void(*pfnQueryClientCvarValue)(const edict_t *player, const char *cvarName);
	void(*pfnQueryClientCvarValue2)(const edict_t *player, const char *cvarName, int requestID);
	int(*pfnCheckParm)(const char *pchCmdLineToken, char **ppnext);
} enginefuncs_t;

extern enginefuncs_t EngineFuncs;
extern globalvars_t *GlobalVars;

// edict->flags
#define	FL_FLY					(1<<0)	// Changes the SV_Movestep() behavior to not need to be on ground
#define	FL_SWIM					(1<<1)	// Changes the SV_Movestep() behavior to not need to be on ground (but stay in water)
#define	FL_CONVEYOR				(1<<2)
#define	FL_CLIENT				(1<<3)
#define	FL_INWATER				(1<<4)
#define	FL_MONSTER				(1<<5)
#define	FL_GODMODE				(1<<6)
#define	FL_NOTARGET				(1<<7)
#define	FL_SKIPLOCALHOST		(1<<8)	// Don't send entity to local host, it's predicting this entity itself
#define	FL_ONGROUND				(1<<9)	// At rest / on the ground
#define	FL_PARTIALGROUND		(1<<10)	// not all corners are valid
#define	FL_WATERJUMP			(1<<11)	// player jumping out of water
#define FL_FROZEN				(1<<12) // Player is frozen for 3rd person camera
#define FL_FAKECLIENT			(1<<13)	// JAC: fake client, simulated server side; don't send network messages to them
#define FL_DUCKING				(1<<14)	// Player flag -- Player is fully crouched
#define FL_FLOAT				(1<<15)	// Apply floating force to this entity when in water
#define FL_GRAPHED				(1<<16) // worldgraph has this ent listed as something that blocks a connection

// UNDONE: Do we need these?
#define FL_IMMUNE_WATER			(1<<17)
#define	FL_IMMUNE_SLIME			(1<<18)
#define FL_IMMUNE_LAVA			(1<<19)

#define FL_PROXY				(1<<20)	// This is a spectator proxy
#define FL_ALWAYSTHINK			(1<<21)	// Brush model flag -- call think every frame regardless of nextthink - ltime (for constantly changing velocity/path)
#define FL_BASEVELOCITY			(1<<22)	// Base velocity has been applied this frame (used to convert base velocity into momentum)
#define FL_MONSTERCLIP			(1<<23)	// Only collide in with monsters who have FL_MONSTERCLIP set
#define FL_ONTRAIN				(1<<24) // Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_WORLDBRUSH			(1<<25)	// Not moveable/removeable brush entity (really part of the world, but represented as an entity for transparency or something)
#define FL_SPECTATOR            (1<<26) // This client is a spectator, don't run touch functions, etc.
#define FL_CUSTOMENTITY			(1<<29)	// This is a custom entity
#define FL_KILLME				(1<<30)	// This entity is marked for death -- This allows the engine to kill ents at the appropriate time
#define FL_DORMANT				(1<<31)	// Entity is dormant, no updates to client


// Goes into globalvars_t.trace_flags
#define FTRACE_SIMPLEBOX		(1<<0)	// Traceline with a simple box


// walkmove modes
#define	WALKMOVE_NORMAL		0 // normal walkmove
#define WALKMOVE_WORLDONLY	1 // doesn't hit ANY entities, no matter what the solid type
#define WALKMOVE_CHECKONLY	2 // move, but don't touch triggers

// edict->movetype values
#define	MOVETYPE_NONE			0		// never moves
//#define	MOVETYPE_ANGLENOCLIP	1
//#define	MOVETYPE_ANGLECLIP		2
#define	MOVETYPE_WALK			3		// Player only - moving on the ground
#define	MOVETYPE_STEP			4		// gravity, special edge handling -- monsters use this
#define	MOVETYPE_FLY			5		// No gravity, but still collides with stuff
#define	MOVETYPE_TOSS			6		// gravity/collisions
#define	MOVETYPE_PUSH			7		// no clip to world, push and crush
#define	MOVETYPE_NOCLIP			8		// No gravity, no collisions, still do velocity/avelocity
#define	MOVETYPE_FLYMISSILE		9		// extra size to monsters
#define	MOVETYPE_BOUNCE			10		// Just like Toss, but reflect velocity when contacting surfaces
#define MOVETYPE_BOUNCEMISSILE	11		// bounce w/o gravity
#define MOVETYPE_FOLLOW			12		// track movement of aiment
#define	MOVETYPE_PUSHSTEP		13		// BSP model that needs physics/world collisions (uses nearest hull for world collision)

// edict->solid values
// NOTE: Some movetypes will cause collisions independent of SOLID_NOT/SOLID_TRIGGER when the entity moves
// SOLID only effects OTHER entities colliding with this one when they move - UGH!
#define	SOLID_NOT				0		// no interaction with other objects
#define	SOLID_TRIGGER			1		// touch on edge, but not blocking
#define	SOLID_BBOX				2		// touch on edge, block
#define	SOLID_SLIDEBOX			3		// touch on edge, but not an onground
#define	SOLID_BSP				4		// bsp clip, touch on edge, block

// edict->deadflag values
#define	DEAD_NO					0 // alive
#define	DEAD_DYING				1 // playing death animation or still falling off of a ledge waiting to hit ground
#define	DEAD_DEAD				2 // dead. lying still.
#define DEAD_RESPAWNABLE		3
#define DEAD_DISCARDBODY		4

#define	DAMAGE_NO				0
#define	DAMAGE_YES				1
#define	DAMAGE_AIM				2

// entity effects
#define	EF_BRIGHTFIELD			1	// swirling cloud of particles
#define	EF_MUZZLEFLASH 			2	// single frame ELIGHT on entity attachment 0
#define	EF_BRIGHTLIGHT 			4	// DLIGHT centered at entity origin
#define	EF_DIMLIGHT 			8	// player flashlight
#define EF_INVLIGHT				16	// get lighting from ceiling
#define EF_NOINTERP				32	// don't interpolate the next frame
#define EF_LIGHT				64	// rocket flare glow sprite
#define EF_NODRAW				128	// don't draw entity
#define EF_NIGHTVISION			256 // player nightvision
#define EF_SNIPERLASER			512 // sniper laser effect
#define EF_FIBERCAMERA			1024// fiber camera


// entity flags
#define EFLAG_SLERP				1	// do studio interpolation of this entity

//
// temp entity events
//
#define	TE_BEAMPOINTS		0		// beam effect between two points
// coord coord coord (start position) 
// coord coord coord (end position) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define	TE_BEAMENTPOINT		1		// beam effect between point and entity
// short (start entity) 
// coord coord coord (end position) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define	TE_GUNSHOT			2		// particle effect plus ricochet sound
// coord coord coord (position) 

#define	TE_EXPLOSION		3		// additive sprite, 2 dynamic lights, flickering particles, explosion sound, move vertically 8 pps
// coord coord coord (position) 
// short (sprite index)
// byte (scale in 0.1's)
// byte (framerate)
// byte (flags)
//
// The Explosion effect has some flags to control performance/aesthetic features:
#define TE_EXPLFLAG_NONE		0	// all flags clear makes default Half-Life explosion
#define TE_EXPLFLAG_NOADDITIVE	1	// sprite will be drawn opaque (ensure that the sprite you send is a non-additive sprite)
#define TE_EXPLFLAG_NODLIGHTS	2	// do not render dynamic lights
#define TE_EXPLFLAG_NOSOUND		4	// do not play client explosion sound
#define TE_EXPLFLAG_NOPARTICLES	8	// do not draw particles


#define	TE_TAREXPLOSION		4		// Quake1 "tarbaby" explosion with sound
// coord coord coord (position) 

#define	TE_SMOKE			5		// alphablend sprite, move vertically 30 pps
// coord coord coord (position) 
// short (sprite index)
// byte (scale in 0.1's)
// byte (framerate)

#define	TE_TRACER			6		// tracer effect from point to point
// coord, coord, coord (start) 
// coord, coord, coord (end)

#define	TE_LIGHTNING		7		// TE_BEAMPOINTS with simplified parameters
// coord, coord, coord (start) 
// coord, coord, coord (end) 
// byte (life in 0.1's) 
// byte (width in 0.1's) 
// byte (amplitude in 0.01's)
// short (sprite model index)

#define	TE_BEAMENTS			8		
// short (start entity) 
// short (end entity) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define	TE_SPARKS			9		// 8 random tracers with gravity, ricochet sprite
// coord coord coord (position) 

#define	TE_LAVASPLASH		10		// Quake1 lava splash
// coord coord coord (position) 

#define	TE_TELEPORT			11		// Quake1 teleport splash
// coord coord coord (position) 

#define TE_EXPLOSION2		12		// Quake1 colormaped (base palette) particle explosion with sound
// coord coord coord (position) 
// byte (starting color)
// byte (num colors)

#define TE_BSPDECAL			13		// Decal from the .BSP file 
// coord, coord, coord (x,y,z), decal position (center of texture in world)
// short (texture index of precached decal texture name)
// short (entity index)
// [optional - only included if previous short is non-zero (not the world)] short (index of model of above entity)

#define TE_IMPLOSION		14		// tracers moving toward a point
// coord, coord, coord (position)
// byte (radius)
// byte (count)
// byte (life in 0.1's) 

#define TE_SPRITETRAIL		15		// line of moving glow sprites with gravity, fadeout, and collisions
// coord, coord, coord (start) 
// coord, coord, coord (end) 
// short (sprite index)
// byte (count)
// byte (life in 0.1's) 
// byte (scale in 0.1's) 
// byte (velocity along vector in 10's)
// byte (randomness of velocity in 10's)

#define TE_BEAM				16		// obsolete

#define TE_SPRITE			17		// additive sprite, plays 1 cycle
// coord, coord, coord (position) 
// short (sprite index) 
// byte (scale in 0.1's) 
// byte (brightness)

#define TE_BEAMSPRITE		18		// A beam with a sprite at the end
// coord, coord, coord (start position) 
// coord, coord, coord (end position) 
// short (beam sprite index) 
// short (end sprite index) 

#define TE_BEAMTORUS		19		// screen aligned beam ring, expands to max radius over lifetime
// coord coord coord (center position) 
// coord coord coord (axis and radius) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define TE_BEAMDISK			20		// disk that expands to max radius over lifetime
// coord coord coord (center position) 
// coord coord coord (axis and radius) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define TE_BEAMCYLINDER		21		// cylinder that expands to max radius over lifetime
// coord coord coord (center position) 
// coord coord coord (axis and radius) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define TE_BEAMFOLLOW		22		// create a line of decaying beam segments until entity stops moving
// short (entity:attachment to follow)
// short (sprite index)
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte,byte,byte (color)
// byte (brightness)

#define TE_GLOWSPRITE		23		
// coord, coord, coord (pos) short (model index) byte (scale / 10)

#define TE_BEAMRING			24		// connect a beam ring to two entities
// short (start entity) 
// short (end entity) 
// short (sprite index) 
// byte (starting frame) 
// byte (frame rate in 0.1's) 
// byte (life in 0.1's) 
// byte (line width in 0.1's) 
// byte (noise amplitude in 0.01's) 
// byte,byte,byte (color)
// byte (brightness)
// byte (scroll speed in 0.1's)

#define TE_STREAK_SPLASH	25		// oriented shower of tracers
// coord coord coord (start position) 
// coord coord coord (direction vector) 
// byte (color)
// short (count)
// short (base speed)
// short (ramdon velocity)

#define TE_BEAMHOSE			26		// obsolete

#define TE_DLIGHT			27		// dynamic light, effect world, minor entity effect
// coord, coord, coord (pos) 
// byte (radius in 10's) 
// byte byte byte (color)
// byte (brightness)
// byte (life in 10's)
// byte (decay rate in 10's)

#define TE_ELIGHT			28		// point entity light, no world effect
// short (entity:attachment to follow)
// coord coord coord (initial position) 
// coord (radius)
// byte byte byte (color)
// byte (life in 0.1's)
// coord (decay rate)

#define TE_TEXTMESSAGE		29
// short 1.2.13 x (-1 = center)
// short 1.2.13 y (-1 = center)
// byte Effect 0 = fade in/fade out
// 1 is flickery credits
// 2 is write out (training room)

// 4 bytes r,g,b,a color1	(text color)
// 4 bytes r,g,b,a color2	(effect color)
// ushort 8.8 fadein time
// ushort 8.8  fadeout time
// ushort 8.8 hold time
// optional ushort 8.8 fxtime	(time the highlight lags behing the leading text in effect 2)
// string text message		(512 chars max sz string)
#define TE_LINE				30
// coord, coord, coord		startpos
// coord, coord, coord		endpos
// short life in 0.1 s
// 3 bytes r, g, b

#define TE_BOX				31
// coord, coord, coord		boxmins
// coord, coord, coord		boxmaxs
// short life in 0.1 s
// 3 bytes r, g, b

#define TE_KILLBEAM			99		// kill all beams attached to entity
// short (entity)

#define TE_LARGEFUNNEL		100
// coord coord coord (funnel position)
// short (sprite index) 
// short (flags) 

#define	TE_BLOODSTREAM		101		// particle spray
// coord coord coord (start position)
// coord coord coord (spray vector)
// byte (color)
// byte (speed)

#define	TE_SHOWLINE			102		// line of particles every 5 units, dies in 30 seconds
// coord coord coord (start position)
// coord coord coord (end position)

#define TE_BLOOD			103		// particle spray
// coord coord coord (start position)
// coord coord coord (spray vector)
// byte (color)
// byte (speed)

#define TE_DECAL			104		// Decal applied to a brush entity (not the world)
// coord, coord, coord (x,y,z), decal position (center of texture in world)
// byte (texture index of precached decal texture name)
// short (entity index)

#define TE_FIZZ				105		// create alpha sprites inside of entity, float upwards
// short (entity)
// short (sprite index)
// byte (density)

#define TE_MODEL			106		// create a moving model that bounces and makes a sound when it hits
// coord, coord, coord (position) 
// coord, coord, coord (velocity)
// angle (initial yaw)
// short (model index)
// byte (bounce sound type)
// byte (life in 0.1's)

#define TE_EXPLODEMODEL		107		// spherical shower of models, picks from set
// coord, coord, coord (origin)
// coord (velocity)
// short (model index)
// short (count)
// byte (life in 0.1's)

#define TE_BREAKMODEL		108		// box of models or sprites
// coord, coord, coord (position)
// coord, coord, coord (size)
// coord, coord, coord (velocity)
// byte (random velocity in 10's)
// short (sprite or model index)
// byte (count)
// byte (life in 0.1 secs)
// byte (flags)

#define TE_GUNSHOTDECAL		109		// decal and ricochet sound
// coord, coord, coord (position)
// short (entity index???)
// byte (decal???)

#define TE_SPRITE_SPRAY		110		// spay of alpha sprites
// coord, coord, coord (position)
// coord, coord, coord (velocity)
// short (sprite index)
// byte (count)
// byte (speed)
// byte (noise)

#define TE_ARMOR_RICOCHET	111		// quick spark sprite, client ricochet sound. 
// coord, coord, coord (position)
// byte (scale in 0.1's)

#define TE_PLAYERDECAL		112		// ???
// byte (playerindex)
// coord, coord, coord (position)
// short (entity???)
// byte (decal number???)
// [optional] short (model index???)

#define TE_BUBBLES			113		// create alpha sprites inside of box, float upwards
// coord, coord, coord (min start position)
// coord, coord, coord (max start position)
// coord (float height)
// short (model index)
// byte (count)
// coord (speed)

#define TE_BUBBLETRAIL		114		// create alpha sprites along a line, float upwards
// coord, coord, coord (min start position)
// coord, coord, coord (max start position)
// coord (float height)
// short (model index)
// byte (count)
// coord (speed)

#define TE_BLOODSPRITE		115		// spray of opaque sprite1's that fall, single sprite2 for 1..2 secs (this is a high-priority tent)
// coord, coord, coord (position)
// short (sprite1 index)
// short (sprite2 index)
// byte (color)
// byte (scale)

#define TE_WORLDDECAL		116		// Decal applied to the world brush
// coord, coord, coord (x,y,z), decal position (center of texture in world)
// byte (texture index of precached decal texture name)

#define TE_WORLDDECALHIGH	117		// Decal (with texture index > 256) applied to world brush
// coord, coord, coord (x,y,z), decal position (center of texture in world)
// byte (texture index of precached decal texture name - 256)

#define TE_DECALHIGH		118		// Same as TE_DECAL, but the texture index was greater than 256
// coord, coord, coord (x,y,z), decal position (center of texture in world)
// byte (texture index of precached decal texture name - 256)
// short (entity index)

#define TE_PROJECTILE		119		// Makes a projectile (like a nail) (this is a high-priority tent)
// coord, coord, coord (position)
// coord, coord, coord (velocity)
// short (modelindex)
// byte (life)
// byte (owner)  projectile won't collide with owner (if owner == 0, projectile will hit any client).

#define TE_SPRAY			120		// Throws a shower of sprites or models
// coord, coord, coord (position)
// coord, coord, coord (direction)
// short (modelindex)
// byte (count)
// byte (speed)
// byte (noise)
// byte (rendermode)

#define TE_PLAYERSPRITES	121		// sprites emit from a player's bounding box (ONLY use for players!)
// byte (playernum)
// short (sprite modelindex)
// byte (count)
// byte (variance) (0 = no variance in size) (10 = 10% variance in size)

#define TE_PARTICLEBURST	122		// very similar to lavasplash.
// coord (origin)
// short (radius)
// byte (particle color)
// byte (duration * 10) (will be randomized a bit)

#define TE_FIREFIELD			123		// makes a field of fire.
// coord (origin)
// short (radius) (fire is made in a square around origin. -radius, -radius to radius, radius)
// short (modelindex)
// byte (count)
// byte (flags)
// byte (duration (in seconds) * 10) (will be randomized a bit)
//
// to keep network traffic low, this message has associated flags that fit into a byte:
#define TEFIRE_FLAG_ALLFLOAT	1 // all sprites will drift upwards as they animate
#define TEFIRE_FLAG_SOMEFLOAT	2 // some of the sprites will drift upwards. (50% chance)
#define TEFIRE_FLAG_LOOP		4 // if set, sprite plays at 15 fps, otherwise plays at whatever rate stretches the animation over the sprite's duration.
#define TEFIRE_FLAG_ALPHA		8 // if set, sprite is rendered alpha blended at 50% else, opaque
#define TEFIRE_FLAG_PLANAR		16 // if set, all fire sprites have same initial Z instead of randomly filling a cube. 
#define TEFIRE_FLAG_ADDITIVE	32 // if set, sprite is rendered non-opaque with additive

#define TE_PLAYERATTACHMENT			124 // attaches a TENT to a player (this is a high-priority tent)
// byte (entity index of player)
// coord (vertical offset) ( attachment origin.z = player origin.z + vertical offset )
// short (model index)
// short (life * 10 );

#define TE_KILLPLAYERATTACHMENTS	125 // will expire all TENTS attached to a player.
// byte (entity index of player)

#define TE_MULTIGUNSHOT				126 // much more compact shotgun message
// This message is used to make a client approximate a 'spray' of gunfire.
// Any weapon that fires more than one bullet per frame and fires in a bit of a spread is
// a good candidate for MULTIGUNSHOT use. (shotguns)
//
// NOTE: This effect makes the client do traces for each bullet, these client traces ignore
//		 entities that have studio models.Traces are 4096 long.
//
// coord (origin)
// coord (origin)
// coord (origin)
// coord (direction)
// coord (direction)
// coord (direction)
// coord (x noise * 100)
// coord (y noise * 100)
// byte (count)
// byte (bullethole decal texture index)

#define TE_USERTRACER				127 // larger message than the standard tracer, but allows some customization.
// coord (origin)
// coord (origin)
// coord (origin)
// coord (velocity)
// coord (velocity)
// coord (velocity)
// byte ( life * 10 )
// byte ( color ) this is an index into an array of color vectors in the engine. (0 - )
// byte ( length * 10 )



#define	MSG_BROADCAST		0		// unreliable to all
#define	MSG_ONE				1		// reliable to one (msg_entity)
#define	MSG_ALL				2		// reliable to all
#define	MSG_INIT			3		// write to the init string
#define MSG_PVS				4		// Ents in PVS of org
#define MSG_PAS				5		// Ents in PAS of org
#define MSG_PVS_R			6		// Reliable to PVS
#define MSG_PAS_R			7		// Reliable to PAS
#define MSG_ONE_UNRELIABLE	8		// Send to one client, but don't put in reliable stream, put in unreliable datagram ( could be dropped )
#define	MSG_SPEC			9		// Sends to all spectator proxies

// contents of a spot in the world
#define	CONTENTS_EMPTY		-1
#define	CONTENTS_SOLID		-2
#define	CONTENTS_WATER		-3
#define	CONTENTS_SLIME		-4
#define	CONTENTS_LAVA		-5
#define	CONTENTS_SKY		-6
/* These additional contents constants are defined in bspfile.h
#define	CONTENTS_ORIGIN		-7		// removed at csg time
#define	CONTENTS_CLIP		-8		// changed to contents_solid
#define	CONTENTS_CURRENT_0		-9
#define	CONTENTS_CURRENT_90		-10
#define	CONTENTS_CURRENT_180	-11
#define	CONTENTS_CURRENT_270	-12
#define	CONTENTS_CURRENT_UP		-13
#define	CONTENTS_CURRENT_DOWN	-14

#define CONTENTS_TRANSLUCENT	-15
*/
#define	CONTENTS_LADDER				-16

#define	CONTENT_FLYFIELD			-17
#define	CONTENT_GRAVITY_FLYFIELD	-18
#define	CONTENT_FOG					-19

#define CONTENT_EMPTY	-1
#define CONTENT_SOLID	-2
#define	CONTENT_WATER	-3
#define CONTENT_SLIME	-4
#define CONTENT_LAVA	-5
#define CONTENT_SKY		-6

// channels
#define CHAN_AUTO			0
#define CHAN_WEAPON			1
#define	CHAN_VOICE			2
#define CHAN_ITEM			3
#define	CHAN_BODY			4
#define CHAN_STREAM			5			// allocate stream channel from the static or dynamic area
#define CHAN_STATIC			6			// allocate channel from the static area 
#define CHAN_NETWORKVOICE_BASE	7		// voice data coming across the network
#define CHAN_NETWORKVOICE_END	500		// network voice data reserves slots (CHAN_NETWORKVOICE_BASE through CHAN_NETWORKVOICE_END).
#define CHAN_BOT			501			// channel used for bot chatter.

// attenuation values
#define ATTN_NONE		0
#define	ATTN_NORM		(float)0.8
#define ATTN_IDLE		(float)2
#define ATTN_STATIC		(float)1.25 

// pitch values
#define	PITCH_NORM		100			// non-pitch shifted
#define PITCH_LOW		95			// other values are possible - 0-255, where 255 is very high
#define PITCH_HIGH		120

// volume values
#define VOL_NORM		1.0

// plats
#define	PLAT_LOW_TRIGGER	1

// Trains
#define	SF_TRAIN_WAIT_RETRIGGER	1
#define SF_TRAIN_START_ON		4		// Train is initially moving
#define SF_TRAIN_PASSABLE		8		// Train is not solid -- used to make water trains

// Break Model Defines

#define BREAK_TYPEMASK	0x4F
#define BREAK_GLASS		0x01
#define BREAK_METAL		0x02
#define BREAK_FLESH		0x04
#define BREAK_WOOD		0x08

#define BREAK_SMOKE		0x10
#define BREAK_TRANS		0x20
#define BREAK_CONCRETE	0x40
#define BREAK_2			0x80

// Colliding temp entity sounds

#define BOUNCE_GLASS	BREAK_GLASS
#define	BOUNCE_METAL	BREAK_METAL
#define BOUNCE_FLESH	BREAK_FLESH
#define BOUNCE_WOOD		BREAK_WOOD
#define BOUNCE_SHRAP	0x10
#define BOUNCE_SHELL	0x20
#define	BOUNCE_CONCRETE BREAK_CONCRETE
#define BOUNCE_SHOTSHELL 0x80

// Temp entity bounce sound types
#define TE_BOUNCE_NULL		0
#define TE_BOUNCE_SHELL		1
#define TE_BOUNCE_SHOTSHELL	2

// Rendering constants
enum
{
	kRenderNormal,			// src
	kRenderTransColor,		// c*a+dest*(1-a)
	kRenderTransTexture,	// src*a+dest*(1-a)
	kRenderGlow,			// src*a+dest -- No Z buffer checks
	kRenderTransAlpha,		// src*srca+dest*(1-srca)
	kRenderTransAdd,		// src*a+dest
};

enum
{
	kRenderFxNone = 0,
	kRenderFxPulseSlow,
	kRenderFxPulseFast,
	kRenderFxPulseSlowWide,
	kRenderFxPulseFastWide,
	kRenderFxFadeSlow,
	kRenderFxFadeFast,
	kRenderFxSolidSlow,
	kRenderFxSolidFast,
	kRenderFxStrobeSlow,
	kRenderFxStrobeFast,
	kRenderFxStrobeFaster,
	kRenderFxFlickerSlow,
	kRenderFxFlickerFast,
	kRenderFxNoDissipation,
	kRenderFxDistort,			// Distort/scale/translate flicker
	kRenderFxHologram,			// kRenderFxDistort + distance fade
	kRenderFxDeadPlayer,		// kRenderAmt is the player index
	kRenderFxExplode,			// Scale up really big!
	kRenderFxGlowShell,			// Glowing Shell
	kRenderFxClampMinScale,		// Keep this sprite from getting very small (SPRITES only!)
	kRenderFxLightMultiplier,   //CTM !!!CZERO added to tell the studiorender that the value in iuser2 is a lightmultiplier
};


typedef unsigned int func_t;
typedef unsigned int string_t;

typedef unsigned char byte;
typedef unsigned short word;
#define _DEF_BYTE_

typedef struct
{
	byte r, g, b;
} color24;

typedef struct
{
	unsigned r, g, b, a;
} colorVec;

#ifdef _WIN32
#pragma pack(push,2)
#endif

typedef struct
{
	unsigned short r, g, b, a;
} PackedColorVec;

#ifdef _WIN32
#pragma pack(pop)
#endif

typedef struct
{
	vec3_t	normal;
	float	dist;
} plane_t;

typedef struct
{
	qboolean	allsolid;	// if true, plane is not valid
	qboolean	startsolid;	// if true, the initial point was in a solid area
	qboolean	inopen, inwater;
	float	fraction;		// time completed, 1.0 = didn't hit anything
	vec3_t	endpos;			// final position
	plane_t	plane;			// surface normal at impact
	edict_t	*ent;			// entity the surface is on
	int		hitgroup;		// 0 == generic, non zero is specific body part
} trace_t;

// Use this instead of ALLOC_STRING on constant strings
#define STRING(offset)		((const char *)(GlobalVars->pStringBase + (unsigned int)(offset)))
#define MAKE_STRING(str)	((uint64_t)(str) - (uint64_t)(STRING(0)))

// Keeps clutter down a bit, when writing key-value pairs
#define WRITEKEY_INT(pf, szKeyName, iKeyValue) ENGINE_FSERVER_PRINT(pf, "\"%s\" \"%d\"", szKeyName, iKeyValue)
#define WRITEKEY_FLOAT(pf, szKeyName, flKeyValue)								\
		ENGINE_FSERVER_PRINT(pf, "\"%s\" \"%f\"", szKeyName, flKeyValue)
#define WRITEKEY_STRING(pf, szKeyName, szKeyValue)								\
		ENGINE_FSERVER_PRINT(pf, "\"%s\" \"%s\"", szKeyName, szKeyValue)
#define WRITEKEY_VECTOR(pf, szKeyName, flX, flY, flZ)							\
		ENGINE_FSERVER_PRINT(pf, "\"%s\" \"%f %f %f\"", szKeyName, flX, flY, flZ)

// Keeps clutter down a bit, when using a float as a bit-vector
#define SetBits(flBitVector, bits)		((flBitVector) = (int)(flBitVector) | (bits))
#define ClearBits(flBitVector, bits)	((flBitVector) = (int)(flBitVector) & ~(bits))
#define FBitSet(flBitVector, bit)		((int)(flBitVector) & (bit))

// Makes these more explicit, and easier to find
#define FILE_GLOBAL static
#define DLL_GLOBAL

// Until we figure out why "const" gives the compiler problems, we'll just have to use
// this bogus "empty" define to mark things as constant.
#define CONSTANT

// More explicit than "int"
typedef int EOFFSET;

// In case it's not alread defined
typedef int BOOL;

// In case this ever changes
#ifndef M_PI
#define M_PI			3.14159265358979323846
#endif

#define cchMapNameMost 32

// Dot products for view cone checking
#define VIEW_FIELD_FULL		(float)-1.0 // +-180 degrees
#define	VIEW_FIELD_WIDE		(float)-0.7 // +-135 degrees 0.1 // +-85 degrees, used for full FOV checks 
#define	VIEW_FIELD_NARROW	(float)0.7 // +-45 degrees, more narrow check used to set up ranged attacks
#define	VIEW_FIELD_ULTRA_NARROW	(float)0.9 // +-25 degrees, more narrow check used to set up ranged attacks

// All monsters need this data
#define		DONT_BLEED			-1
#define		BLOOD_COLOR_RED		(BYTE)247
#define		BLOOD_COLOR_YELLOW	(BYTE)195
#define		BLOOD_COLOR_GREEN	BLOOD_COLOR_YELLOW

typedef enum
{

	MONSTERSTATE_NONE = 0,
	MONSTERSTATE_IDLE,
	MONSTERSTATE_COMBAT,
	MONSTERSTATE_ALERT,
	MONSTERSTATE_HUNT,
	MONSTERSTATE_PRONE,
	MONSTERSTATE_SCRIPT,
	MONSTERSTATE_PLAYDEAD,
	MONSTERSTATE_DEAD

} MONSTERSTATE;

// Things that toggle (buttons/triggers/doors) need this
typedef enum
{
	TS_AT_TOP,
	TS_AT_BOTTOM,
	TS_GOING_UP,
	TS_GOING_DOWN
} TOGGLE_STATE;

//
// Constants that were used only by QC (maybe not used at all now)
//
// Un-comment only as needed
//
#define LANGUAGE_ENGLISH				0
#define LANGUAGE_GERMAN					1
#define LANGUAGE_FRENCH					2
#define LANGUAGE_BRITISH				3

extern DLL_GLOBAL int			g_Language;

#define AMBIENT_SOUND_STATIC			0	// medium radius attenuation
#define AMBIENT_SOUND_EVERYWHERE		1
#define AMBIENT_SOUND_SMALLRADIUS		2
#define AMBIENT_SOUND_MEDIUMRADIUS		4
#define AMBIENT_SOUND_LARGERADIUS		8
#define AMBIENT_SOUND_START_SILENT		16
#define AMBIENT_SOUND_NOT_LOOPING		32

#define SPEAKER_START_SILENT			1	// wait for trigger 'on' to start announcements

#define SND_SPAWNING		(1<<8)		// duplicated in protocol.h we're spawing, used in some cases for ambients 
#define SND_STOP			(1<<5)		// duplicated in protocol.h stop sound
#define SND_CHANGE_VOL		(1<<6)		// duplicated in protocol.h change sound vol
#define SND_CHANGE_PITCH	(1<<7)		// duplicated in protocol.h change sound pitch

#define	LFO_SQUARE			1
#define LFO_TRIANGLE		2
#define LFO_RANDOM			3

// func_rotating
#define SF_BRUSH_ROTATE_Y_AXIS		0
#define SF_BRUSH_ROTATE_INSTANT		1
#define SF_BRUSH_ROTATE_BACKWARDS	2
#define SF_BRUSH_ROTATE_Z_AXIS		4
#define SF_BRUSH_ROTATE_X_AXIS		8
#define SF_PENDULUM_AUTO_RETURN		16
#define	SF_PENDULUM_PASSABLE		32


#define SF_BRUSH_ROTATE_SMALLRADIUS	128
#define SF_BRUSH_ROTATE_MEDIUMRADIUS 256
#define SF_BRUSH_ROTATE_LARGERADIUS 512

#define PUSH_BLOCK_ONLY_X	1
#define PUSH_BLOCK_ONLY_Y	2

#define VEC_HULL_MIN		Vector(-16, -16, -36)
#define VEC_HULL_MAX		Vector( 16,  16,  36)
#define VEC_HUMAN_HULL_MIN	Vector( -16, -16, 0 )
#define VEC_HUMAN_HULL_MAX	Vector( 16, 16, 72 )
#define VEC_HUMAN_HULL_DUCK	Vector( 16, 16, 36 )

#define VEC_VIEW			Vector( 0, 0, 28 )

#define VEC_DUCK_HULL_MIN	Vector(-16, -16, -18 )
#define VEC_DUCK_HULL_MAX	Vector( 16,  16,  18)
#define VEC_DUCK_VIEW		Vector( 0, 0, 12 )

#define SVC_TEMPENTITY		23
#define SVC_INTERMISSION	30
#define SVC_CDTRACK			32
#define SVC_WEAPONANIM		35
#define SVC_ROOMTYPE		37
#define	SVC_DIRECTOR		51



// triggers
#define	SF_TRIGGER_ALLOWMONSTERS	1// monsters allowed to fire this trigger
#define	SF_TRIGGER_NOCLIENTS		2// players not allowed to fire this trigger
#define SF_TRIGGER_PUSHABLES		4// only pushables can fire this trigger

// func breakable
#define SF_BREAK_TRIGGER_ONLY	1// may only be broken by trigger
#define	SF_BREAK_TOUCH			2// can be 'crashed through' by running player (plate glass)
#define SF_BREAK_PRESSURE		4// can be broken by a player standing on it
#define SF_BREAK_CROWBAR		256// instant break if hit with crowbar

// func_pushable (it's also func_breakable, so don't collide with those flags)
#define SF_PUSH_BREAKABLE		128

#define SF_LIGHT_START_OFF		1

#define SPAWNFLAG_NOMESSAGE	1
#define SPAWNFLAG_NOTOUCH	1
#define SPAWNFLAG_DROIDONLY	4

#define SPAWNFLAG_USEONLY	1		// can't be touched, must be used (buttons)

#define TELE_PLAYER_ONLY	1
#define TELE_SILENT			2

#define SF_TRIG_PUSH_ONCE		1


// Sound Utilities

// sentence groups
#define CBSENTENCENAME_MAX 16
#define CVOXFILESENTENCEMAX		1536		// max number of sentences in game. NOTE: this must match
// CVOXFILESENTENCEMAX in engine\sound.h!!!

#define GROUP_OP_AND	0
#define GROUP_OP_NAND	1

#define FixedSigned16(X, Y) (short)(X * (Y))
#define FixedUnsigned16(X, Y) (USHORT)(X * (Y))
#define fast_FNullEnt(X) (!X || !X->pvPrivateData)