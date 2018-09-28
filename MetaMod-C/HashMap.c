#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "HashMap.h"

#define HM_GetBucketIndex(HashMap, Hash) Hash & (HashMap->BucketCount - 1)

void HM_Clear(StringHashMap *HashMap)
{
	StringHashNode *Next, *HashNode;
	int Index = HashMap->BucketCount;

	while (Index--)
	{
		HashNode = HashMap->Buckets[Index];

		while (HashNode)
		{
			Next = HashNode->Next;
			free(HashNode);
			HashNode = Next;
		}
	}

	free(HashMap->Buckets);
}

BOOL HM_Retrieve(StringHashMap *HashMap, const char *Key, int *Output)
{
	StringHashNode *Result = HM_GetReference(HashMap, Key);
	
	if (Result)
	{
		if (Output)
			*Output = Result->Value;
		
		return TRUE;
	}

	return FALSE;
}

int HM_Insert(StringHashMap *HashMap, const char *Key, int Value)
{
	int Index;
	StringHashNode *NextHashNode, *HashNode;

	NextHashNode = HM_GetReference(HashMap, Key);

	if (NextHashNode)
	{
		NextHashNode->Value = Value;
		return 0;
	}

	HashNode = HM_CreateNode(Key, Value);

	if (HashNode == NULL)
		goto Fail;

	if (HashMap->NodeCount >= HashMap->BucketCount)
	{
		Index = (HashMap->BucketCount > 0) ? (HashMap->BucketCount << 1) : 1;

		if (HM_Resize(HashMap, Index))
			goto Fail;
	}

	HM_AddNode(HashMap, HashNode);
	HashMap->NodeCount++;
	return 0;

Fail:
	if (HashNode)
		free(HashNode);

	return -1;
}

void HM_Remove(StringHashMap *HashMap, const char *Key)
{
	StringHashNode *HashNode;
	StringHashNode *Next = HM_GetReference(HashMap, Key);

	if (Next)
	{
		HashNode = Next;
		Next = Next->Next;
		free(HashNode);
		HashMap->NodeCount--;
	}
}

uint32_t HM_HashValue(const char *Key)
{
	uint32_t Hash = 0;

	while (*Key)
		Hash = Hash * 101 + *Key++;

	return Hash;
}

static StringHashNode *HM_CreateNode(const char *Key, int Value)
{
	StringHashNode *HashNode;
	HashNode = malloc(sizeof(StringHashNode));
	HashNode->Key = strdup(Key);
	HashNode->Value = Value;
	HashNode->Hash = HM_HashValue(Key);
	return HashNode;
}

static void HM_AddNode(StringHashMap *HashMap, StringHashNode *HashNode)
{
	int Index = HM_GetBucketIndex(HashMap, HashNode->Hash);
	HashNode->Next = HashMap->Buckets[Index];
	HashMap->Buckets[Index] = HashNode;
}

static int HM_Resize(StringHashMap *HashMap, int BucketCount)
{
	StringHashNode *HashNodes, *HashNode, *Next;
	StringHashNode **_Buckets;
	int Index = HashMap->BucketCount;

	HashNodes = NULL;

	while (Index--)
	{
		HashNode = (HashMap->Buckets)[Index];

		while (HashNode)
		{
			Next = HashNode->Next;
			HashNode->Next = HashNodes;
			HashNodes = HashNode;
			HashNode = Next;
		}
	}

	_Buckets = (StringHashNode **)realloc(HashMap->Buckets, sizeof(*HashMap->Buckets) * BucketCount);

	if (_Buckets != NULL)
	{
		HashMap->Buckets = _Buckets;
		HashMap->BucketCount = BucketCount;
	}

	if (HashMap->Buckets)
	{
		memset(HashMap->Buckets, 0, sizeof(*HashMap->Buckets) * HashMap->BucketCount);

		HashNode = HashNodes;

		while (HashNode)
		{
			Next = HashNode->Next;
			HM_AddNode(HashMap, HashNode);
			HashNode = Next;
		}
	}

	return (_Buckets == NULL) ? -1 : 0;
}

static StringHashNode *HM_GetReference(StringHashMap *HashMap, const char *Key)
{
	uint32_t Hash = HM_HashValue(Key);
	StringHashNode *Next;

	if (HashMap->BucketCount > 0)
	{
		Next = HashMap->Buckets[HM_GetBucketIndex(HashMap, Hash)];
		
		while (Next)
		{
			if (Next->Hash == Hash && !strcmp(Next->Key, Key))
				return Next;

			Next = Next->Next;
		}
	}

	return NULL;
}