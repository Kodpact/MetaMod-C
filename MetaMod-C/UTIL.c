#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <Windows.h>

#include "Types.h"
#include "UTIL.h"
#include "HashMap.h"

#define LocateValidCharacter(Buffer) while (*Buffer > 0 && *Buffer < 33) Buffer++;
#define CheckEOF(Buffer, Ret) if (*Buffer == 0) return Ret;
#define ReadWord(Buffer, InQuotes) while (*Buffer){ if (*Buffer < 33 && !InQuotes) break; if (*Buffer == '"') InQuotes = !InQuotes; Buffer++; }

int ReadKeyValue(char *Buffer, char **Key, char **Value, char **RealBuffer)
{
	char *Start = Buffer;
	BOOL InQuotes = FALSE;
	LocateValidCharacter(Buffer);
	CheckEOF(Buffer, 1);
	*Key = Buffer;
	ReadWord(Buffer, InQuotes);
	char *End = Buffer;
	LocateValidCharacter(Buffer);
	CheckEOF(Buffer, -1);
	*End = 0;
	*Value = Buffer;
	ReadWord(Buffer, InQuotes);
	*RealBuffer = Buffer;
	return 0;
}

void RemoveQuotes(char **Buffer)
{
	if (**Buffer == '"')
		(*Buffer)++;

	int Len = strlen(*Buffer);

	if ((*Buffer)[Len - 1] == '"')
		(*Buffer)[Len - 1] = 0;
}

void NormalizePathName(char *Path)
{
	while (*Path)
	{
		if (isupper(*Path))
			*Path = tolower(*Path);

		if (*Path == '\\')
			*Path = '/';
		
		Path++;
	}
}

void FileToDirectory(char *Path)
{
	Path += strlen(Path) - 1;

	while (*Path)
	{
		if (*Path == '\\' || *Path == '/')
		{
			*Path = '\0';
			break;
		}

		Path--;
	}
}

int GetPreviousDirectory(char *Path)
{
	int Len = strlen(Path);

	if (Len == 0)
		return -1;

	Path += Len - 1;

	while (*Path)
	{
		if (*Path == '/' || *Path == '\\')
		{
			*Path = 0;
			return 0;
		}

		Path--;
	}

	return -1;
}

char *GetDLLFileName(char *Path)
{
	char *Origin = Path;
	int Len = strlen(Path);

	if (Len == 0)
		return Path;

	Path += Len - 1;

	while (*Path)
	{
		if (*Path == '/' || *Path == '\\')
		{
			if (*(Path + 1) == 0)
				return Path;

			return (Path + 1);
		}

		Path--;
	}

	return Origin;
}

char *GetDLLFileType(char *Path)
{
	int Len = strlen(Path);

	if (Len == 0)
		return Path;

	Path += Len - 1;

	while (*Path)
	{
		if (*Path == '.')
		{
			if (*(Path + 1) == 0)
				return Path;

			return (Path + 1);
		}

		Path--;
	}

	return Path;
}

static va_list Buformat;
static char Buffer[2048];

void ServerPrint(char *Format, ...)
{
	va_start(Buformat, Format);
	int Bytes = vsnprintf(Buffer, 1023, Format, Buformat);
	va_end(Buformat);
	Buffer[Bytes] = '\n';
	Buffer[Bytes + 1] = '\0';
	SERVER_PRINT(Buffer);
}

static HANDLE Console;

void ColoredPrint_Init(void)
{
	Console = GetStdHandle(STD_OUTPUT_HANDLE);
}

void ServerPrintColored(int Type, char *Format, ...)
{
	SetConsoleTextAttribute(Console, Type);
	va_start(Buformat, Format);
	int Bytes = vsnprintf(Buffer, 1023, Format, Buformat);
	va_end(Buformat);
	Buffer[Bytes] = '\n';
	Buffer[Bytes + 1] = '\0';
	SERVER_PRINT(Buffer);
	SetConsoleTextAttribute(Console, 7);
}