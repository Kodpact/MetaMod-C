#pragma once

#include <stdint.h>
#include <Windows.h>

typedef struct
{
	char *Key;
	int Value;
	uint32_t Hash;
	void *Next;
} StringHashNode;

typedef struct
{
	StringHashNode **Buckets;
	int BucketCount;
	int NodeCount;
} StringHashMap;

void HM_Clear(StringHashMap *HashMap);
BOOL HM_Retrieve(StringHashMap *HashMap, const char *Key, int *Output);
int HM_Insert(StringHashMap *HashMap, const char *Key, int Value);
uint32_t HM_HashValue(const char *Key);

static void HM_Remove(StringHashMap *HashMap, const char *Key);
static int HM_Resize(StringHashMap *HashMap, int _BucketCount);
static StringHashNode *HM_GetReference(StringHashMap *HashMap, const char *Key);
static StringHashNode *HM_CreateNode(const char *Key, int Value);
static void HM_AddNode(StringHashMap *HashMap, StringHashNode *HashNode);