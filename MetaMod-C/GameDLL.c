#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

#include "Types.h"
#include "UTIL.h"
#include "HashMap.h"

extern enginefuncs_t EngineFuncs;
extern globalvars_t *GlobalVars;

extern DLL_FUNCTIONS GameDLLFuncs;
extern NEW_DLL_FUNCTIONS GameNewDLLFuncs;

extern char Directory[MAX_PATH];

const GameData KnownData[] =
{
	{ "action", "ahl.dll", "Action Half-Life", 3405141894 },
	{ "ag", "ag.dll", "Adrenaline Gamer Steam", 9900 },
	{ "ag3", "hl.dll", "Adrenalinegamer 3.x", 999951 },
	{ "aghl", "ag.dll", "Adrenalinegamer 4.x", 101000512 },
	{ "arg", "hl.dll", "Arg!", 1001114 },
	{ "asheep", "hl.dll", "Azure Sheep", 762735590 },
	{ "hcfrenzy", "hcfrenzy.dll", "Headcrab Frenzy", 3016778493 },
	{ "bdef", "../cl_dlls/server.dll", "Base Defense [Modification]", 101999901 },
	{ "bdef", "server.dll", "Base Defense [Steam Version]", 101999901 },
	{ "bg", "bg.dll", "The Battle Grounds", 10001 },
	{ "bhl", "Brutal Half-Life", "", 1010310 },
	{ "bot", "bot.dll", "Bot", 1011025 },
	{ "brainbread", "bb.dll", "BrainBread", 542428082 },
	{ "bumpercars", "hl.dll", "Bumper Cars", 1877961328 },
	{ "buzzybots", "bb.dll", "BuzzyBots", 1713374448 },
	{ "ckf3", "mp.dll", "Chicken Fortress 3", 103101659 },
	{ "cs13", "mp.dll", "Counter-Strike 1.3", 103177914 },
	{ "cstrike", "mp.dll", "Counter-Strike", 2315499365 },
	{ "csv15", "mp.dll", "CS 1.5 for Steam", 1831738442 },
	{ "czero", "mp.dll", "Counter-Strike:Condition Zero", 1838783755 },
	{ "dcrisis", "dc.dll", "Desert Crisis", 2103552693 },
	{ "dmc", "dmc.dll", "Deathmatch Classic", 1031208 },
	{ "dod", "dod.dll", "Day of Defeat", 1031411 },
	{ "dpb", "pb.dll", "Digital Paintball", 1031510 },
	{ "dragonmodz", "mp.dll", "Dragon Mod Z", 2327050425 },
	{ "esf", "hl.dll", "Earth's Special Forces", 1042018 },
	{ "existence", "existence.dll", "Existence", 110686740 },
	{ "firearms", "firearms.dll", "Firearms", 2083301025 },
	{ "firearms25", "firearms.dll", "Retro Firearms", 255580520 },
	{ "freeze", "mp.dll", "Freeze", 1666481349 },
	{ "frontline", "frontline.dll", "Frontline Force", 1622144217 },
	{ "gangstawars", "gwars27.dll", "Gangsta Wars", 281645438 },
	{ "gangwars", "mp.dll", "Gangwars", 478926530 },
	{ "gearbox", "opfor.dll", "Opposing Force", 1878675768 },
	{ "globalwarfare", "mp.dll", "Global Warfare", 145526157 },
	{ "goldeneye", "mp.dll", "Goldeneye", 2272306868 },
	{ "hl15we", "Half-Life 1.5: Weapon Edition", "", 533417862 },
	{ "HLAinGOLDSrc", "Half-Life Alpha in GOLDSrc", "", 3312355626 },
	{ "hlrally", "hlrally.dll", "HL-Rally", 554809844 },
	{ "holywars", "holywars.dll", "Holy Wars", 1768514133 },
	{ "hostileintent", "hl.dll", "Hostile Intent", 2668173906 },
	{ "ios", "ios.dll", "International Online Soccer", 1082431 },
	{ "judgedm", "mp.dll", "Judgement", 532154784 },
	{ "kanonball", "kanonball.dll", "Kanonball", 2454010394 },
	{ "monkeystrike", "monkey.dll", "Monkeystrike", 1467157805 },
	{ "MorbidPR", "morbid.dll", "Morbid Inclination", 1848930807 },
	{ "movein", "hl.dll", "Move In!", 1928073030 },
	{ "msc", "ms.dll", "Master Sword Continued", 1123623 },
	{ "ns", "ns.dll", "Natural Selection", 11225 },
	{ "nsp", "ns.dll", "Natural Selection Beta", 1133837 },
	{ "oel", "hl.dll", "OeL Half-Life", 1142620 },
	{ "og", "og.dll", "Over Ground", 11314 },
	{ "ol", "hl.dll", "Outlawsmod", 11319 },
	{ "ops1942", "spirit.dll", "Operations 1942", 47250990 },
	{ "osjb", "jail.dll", "Open-Source Jailbreak", 115547330 },
	{ "outbreak", "hl.dll", "Out Break", 2281947965 },
	{ "oz", "mp.dll", "Oz Deathmatch", 11333 },
	{ "paintball", "mp.dll", "Paintball", 2943689891 },
	{ "penemy", "PE.dll", "Public Enemy", 2344757458 },
	{ "phineas", "phineas.dll", "Phineas Bot", 1551719196 },
	{ "ponreturn", "mp.dll", "Point of No Return", 1015651033 },
	{ "pvk", "hl.dll", "Pirates, Vikings and Knights", 1154537 },
	{ "rc2", "rc2.dll", "Rocket Crowbar 2", 1172963 },
	{ "recbb2", "recb.dll", "Resident Evil : Cold Blood", 1878756884 },
	{ "retrocs", "rcs.dll", "Retro Counter-Strike", 2561533570 },
	{ "rewolf", "gunman.dll", "Gunman Chronicles", 1899496579 },
	{ "ricochet", "mp.dll", "Ricochet", 767222277 },
	{ "rockcrowbar", "rc.dll", "Rocket Crowbar", 3875093591 },
	{ "rspecies", "hl.dll", "Rival Species", 1256299102 },
	{ "scihunt", "shunt.dll", "Scientist Hunt", 2524492434 },
	{ "sdm", "sdmmod.dll", "Special Death Match", 1183324 },
	{ "Ship", "ship.dll", "The Ship", 86586604 },
	{ "si", "si.dll", "Science & Industry", 11720 },
	{ "snow", "snow.dll", "Snow-War", 119618055 },
	{ "stargatetc", "hl.dll", "StargateTC", 3150431054 },
	{ "svencoop", "hl.dll", "Sven Coop [Modification]", 2802202525 },
	{ "svencoop", "server.dll", "Sven Coop [Steam Version]", 2802202525 },
	{ "swarm", "swarm.dll", "Swarm", 3500618462 },
	{ "tfc", "tfc.dll", "Team Fortress Classic", 1193717 },
	{ "thewastes", "thewastes.dll", "The Wastes", 3265367364 },
	{ "timeless", "timeless.dll", "Project Timeless", 2585820294 },
	{ "tod", "hl.dll", "Tour of Duty", 1194627 },
	{ "trainhunters", "th.dll", "Train Hunters", 4172598279 },
	{ "trevenge", "trevenge.dll", "The Terrorist Revenge", 3545389308 },
	{ "TS", "mp.dll", "The Specialists", 8567 },
	{ "tt", "tt.dll", "The Trenches", 11832 },
	{ "underworld", "uw.dll", "Underworld Bloodline", 2264716718 },
	{ "valve", "hl.dll", "Half-Life Deathmatch", 3790245650 },
	{ "vs", "mp.dll", "VampireSlayer", 12033 },
	{ "wantedhl", "wanted.dll", "Wanted!", 4143577431 },
	{ "wasteland", "mp.dll", "Wasteland", 918839271 },
	{ "weapon_wars", "hl.dll", "Weapon Wars", 196734566 },
	{ "wizwars", "hl.dll", "Wizard Wars", 1302810375 },
	{ "wormshl", "wormshl.dll", "WormsHL", 3391267620 },
	{ "zp", "mp.dll", "Zombie Panic", 12434 },
};

GameInfo DLLInfo;
HMODULE GameDLL;
extern IMAGE_DOS_HEADER __ImageBase;

#define RVA_VA(A, B) ((unsigned long)A + (unsigned long)B)
#define VA_RVA(A, B) ((unsigned long)B - (unsigned long)A)

typedef struct
{
	unsigned long Name;
	unsigned short NameOrdinal;
} SortName;

static int SortNameList(const SortName *A, const SortName *B)
{
	return strcmp((const char *)A->Name, (const char *)B->Name);
}

static IMAGE_NT_HEADERS *GetNTHeaders(HMODULE Module)
{
	union
	{
		unsigned long   Memory;
		IMAGE_DOS_HEADER * DOS;
		IMAGE_NT_HEADERS * PE;
	} Memory;

	Memory.Memory = (unsigned long)Module;
	
	if (IsBadReadPtr(Memory.DOS, sizeof(*Memory.DOS)) || Memory.DOS->e_magic != IMAGE_DOS_SIGNATURE)
		return 0;

	Memory.Memory = RVA_VA(Module, Memory.DOS->e_lfanew);
	
	if (IsBadReadPtr(Memory.PE, sizeof(*Memory.PE)) || Memory.PE->Signature != IMAGE_NT_SIGNATURE)
		return 0;

	return Memory.PE;
}

static IMAGE_EXPORT_DIRECTORY *GetExportTable(HMODULE Module)
{
	union
	{
		unsigned long Memory;
		void *Pointer;
		IMAGE_DOS_HEADER *DOS;
		IMAGE_NT_HEADERS *PE;
		IMAGE_EXPORT_DIRECTORY *ExportDir;
	} Memory;

	Memory.PE = GetNTHeaders(Module);

	if (!Memory.PE)
		return 0;

	if (!Memory.PE->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress)
		return 0;

	Memory.Memory = RVA_VA(Module, Memory.PE->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
	
	if (IsBadReadPtr(Memory.ExportDir, sizeof(*Memory.ExportDir)))
		return 0;

	return Memory.ExportDir;
}

static int CombineModuleExportTables(HMODULE MetaModule, HMODULE GameModule)
{
	IMAGE_EXPORT_DIRECTORY *MetaExport;
	IMAGE_EXPORT_DIRECTORY *GameExport;

	unsigned long FunctionCount;
	unsigned long NameCount;
	unsigned long *Functions;
	unsigned long *Names;
	unsigned short *NameOrdinals;
	SortName *Sort;

	unsigned long Index;
	unsigned long Undex;
	unsigned long FCount;
	unsigned long NCount;
	unsigned long ListFix;

	MetaExport = GetExportTable(MetaModule);
	GameExport = GetExportTable(GameModule);

	if (!MetaExport || !GameExport)
	{
		ServerPrintColored(TYPE_ERROR, "Couldn't Initialize Dynamic LinkEnts. (MetaExport: %i, GameExport: %i)", MetaExport, GameExport);
		return -1;
	}

	FunctionCount = MetaExport->NumberOfFunctions + GameExport->NumberOfFunctions;
	NameCount = MetaExport->NumberOfNames + GameExport->NumberOfNames;

	*(void**)&Functions = calloc(1, FunctionCount * sizeof(*Functions));
	*(void**)&Sort = calloc(1, NameCount * sizeof(*Sort));

	for (FCount = 0; FCount < MetaExport->NumberOfFunctions; FCount++)
		Functions[FCount] = RVA_VA(MetaModule, ((unsigned long*)RVA_VA(MetaModule, MetaExport->AddressOfFunctions))[FCount]);
	
	for (NCount = 0; NCount < MetaExport->NumberOfNames; NCount++)
	{
		Sort[NCount].Name = RVA_VA(MetaModule, ((unsigned long*)RVA_VA(MetaModule, MetaExport->AddressOfNames))[NCount]);
		Sort[NCount].NameOrdinal = ((unsigned short *)RVA_VA(MetaModule, MetaExport->AddressOfNameOrdinals))[NCount];
	}

	for (Index = 0; Index < GameExport->NumberOfFunctions; Index++)
		Functions[FCount + Index] = RVA_VA(GameModule, ((unsigned long*)RVA_VA(GameModule, GameExport->AddressOfFunctions))[Index]);
	
	for (Index = 0, ListFix = 0; Index < GameExport->NumberOfNames; Index++)
	{
		const char * Name = (const char *)RVA_VA(GameModule, ((unsigned long*)RVA_VA(GameModule, GameExport->AddressOfNames))[Index]);
		
		for (Undex = 0; Undex < NCount; Undex++)
		{
			if (!_stricmp(Name, (const char*)Sort[Undex].Name))
			{
				ListFix -= 1;
				break;
			}
		}

		if (Undex < NCount)
			continue;

		Sort[NCount + Index + ListFix].Name = (unsigned long)Name;
		Sort[NCount + Index + ListFix].NameOrdinal = (unsigned short)FCount + ((unsigned short *)RVA_VA(GameModule, GameExport->AddressOfNameOrdinals))[Index];
	}

	NameCount = NCount + Index + ListFix;
	qsort(Sort, NameCount, sizeof(*Sort), (int(*)(const void*, const void*))&SortNameList);

	*(void**)&Names = VirtualAlloc(0, NameCount * sizeof(*Names), MEM_COMMIT, PAGE_READWRITE);
	*(void**)&NameOrdinals = VirtualAlloc(0, NameCount * sizeof(*NameOrdinals), MEM_COMMIT, PAGE_READWRITE);

	for (Index = 0; Index < NameCount; Index++)
	{
		Names[Index] = Sort[Index].Name;
		NameOrdinals[Index] = Sort[Index].NameOrdinal;
	}

	free(Sort);

	for (Index = 0; Index < FunctionCount; Index++)
		Functions[Index] = VA_RVA(MetaModule, Functions[Index]);
	
	for (Index = 0; Index < NameCount; Index++)
	{
		Names[Index] = VA_RVA(MetaModule, Names[Index]);
		NameOrdinals[Index] = (unsigned short)VA_RVA(MetaModule, NameOrdinals[Index]);
	}

	DWORD OldProtect;
	if (!VirtualProtect(MetaExport, sizeof(*MetaExport), PAGE_READWRITE, &OldProtect))
	{
		ServerPrintColored(TYPE_ERROR, "Couldn't Initialize Dynamic LinkEnts. (VirtualProtect Failed: %i)", GetLastError());
		return -1;
	}

	MetaExport->Base = 1;
	MetaExport->NumberOfFunctions = FunctionCount;
	MetaExport->NumberOfNames = NameCount;
	*(unsigned long*)&(MetaExport->AddressOfFunctions) = VA_RVA(MetaModule, Functions);
	*(unsigned long*)&(MetaExport->AddressOfNames) = VA_RVA(MetaModule, Names);
	*(unsigned long*)&(MetaExport->AddressOfNameOrdinals) = VA_RVA(MetaModule, NameOrdinals);
	VirtualProtect(MetaExport, sizeof(*MetaExport), OldProtect, &OldProtect);
	return 0;
}

int GameDLL_Init(char *GameDir)
{
	uint32_t Hash = HM_HashValue(GameDir);
	int GameID = -1;

	for (int Index = 0; Index < 95; Index++)
	{
		if (Hash == KnownData[Index].Hash && !strcmp(GameDir, KnownData[Index].Dir))
		{
			GameID = Index;
			break;
		}
	}

	if (GameID < 0)
		return -1;

	char LibPath[MAX_PATH];
	wsprintf(LibPath, "%s/dlls/%s", GameDir, KnownData[GameID].Lib);
	DLLInfo.Name = KnownData[GameID].Dir;
	DLLInfo.GameDir = KnownData[GameID].Dir;
	DLLInfo.Name = KnownData[GameID].Dir;
	DLLInfo.DllFileName = GetDLLFileName(KnownData[GameID].Lib);
	DLLInfo.DllFullPath = strdup(LibPath);
	GameDLL = LoadLibrary(LibPath);
	wsprintf(LibPath, "%s/%s/dlls/%s", Directory, GameDir, KnownData[GameID].Lib);
	DLLInfo.RealDllFullPath = strdup(LibPath);

	if (!GameDLL)
		return -1;

	PFN_GiveFnptrsToDll FGiveFnptrsToDll = (PFN_GiveFnptrsToDll)GetProcAddress(GameDLL, "GiveFnptrsToDll");

	if (!FGiveFnptrsToDll)
		return -1;

	CombineModuleExportTables((HINSTANCE)&__ImageBase, GameDLL);
	
	extern enginefuncs_t MetaEngineFuncs;
	FGiveFnptrsToDll(&MetaEngineFuncs, GlobalVars);
	
	static int IV = INTERFACE_VERSION;
	APIFUNCTION2 FGetAPI2 = (APIFUNCTION2)GetProcAddress(GameDLL, "GetEntityAPI2");

	if (!FGetAPI2)
	{
		APIFUNCTION FGetAPI = (APIFUNCTION)GetProcAddress(GameDLL, "GetEntityAPI");

		if (!FGetAPI)
		{
			ServerPrintColored(TYPE_ERROR, "GameDLL Failed To Load (GetEntityAPI).");
			return -1;
		}

		if (!FGetAPI(&GameDLLFuncs, INTERFACE_VERSION))
		{
			ServerPrintColored(TYPE_ERROR, "GameDLL Failed To Load (GetEntityAPI).");
			return -1;
		}
	}
	else if (!FGetAPI2(&GameDLLFuncs, &IV))
	{
		ServerPrintColored(TYPE_ERROR, "GameDLL Failed To Load (GetEntityAPI2).");
		return -1;
	}
	
	IV = NEW_DLL_FUNCTIONS_VERSION;
	NEW_DLL_FUNCTIONS_FN FNewAPI = (NEW_DLL_FUNCTIONS_FN)GetProcAddress(GameDLL, "GetNewDLLFunctions");

	if (!FNewAPI)
	{
		ServerPrintColored(TYPE_ERROR, "Cannot Get GameDLL Function: GetNewDLLFunctions");
		return -1;
	}

	if (!FNewAPI(&GameNewDLLFuncs, &IV))
	{
		ServerPrintColored(TYPE_ERROR, "GameDLL Failed To Load (Potentially Unsupported Interface Version).");
		return -1;
	}

	return 0;
}

