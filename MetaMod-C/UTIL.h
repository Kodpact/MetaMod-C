#pragma once
#include <stdint.h>

#define TYPE_DEFAULT 11
#define TYPE_INFO 11
#define TYPE_ERROR 12
#define TYPE_WARNING 14

int ReadKeyValue(char *Buffer, char **Key, char **Value, char **RealBuffer);
void RemoveQuotes(char **Buffer);
void NormalizePathName(char *Path);
void FileToDirectory(char *Path);
void FileToDirectory(char *Path);
int GetPreviousDirectory(char *Path);
char *GetDLLFileName(char *Path);
char *GetDLLFileType(char *Path);
void ServerPrint(char *Format, ...);
void ColoredPrint_Init(void);
void ServerPrintColored(int Type, char *Format, ...);