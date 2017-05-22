#pragma once
#include <stdio.h>

#define DATA_READ		0
#define DATA_WRITE		1
#define INST_READ		2

#define HIT			0
#define MISS		1
#define ERROR		-1

#define COMPARE_PENALTY	1
#define L1_MISS_PENALTY	10
#define L2_MISS_PENALTY	100

typedef unsigned long long	ADD64;

enum eChangePolicy {
	RANDOM = 0,
	FIFO,
	LRU,
	PSUEDO
};

enum eCacheLevel {
	eL1 = 0,
	eL2,
	eL3
};

struct DirectedMapLine
{
	bool bValid;
	ADD64 pTag;
	ADD64 pIndex;
	ADD64 cOffest;
};

struct CacheMap
{
	int iAssoIndex;
	bool bDirty;
	struct DirectedMapLine* pBlocks;
};

class cache {
private:
	int Type;
	int AssociateNumber;
	enum eChangePolicy ePolicy;
	int iMissPanelty;
	int iChangePosition;

	int iMainMemorySize;
	int iCacheSize;
	int iBlockSize;
	int iLineNumbers;
	int iLinePosition;

	int iWriteType;

	int iTagLength;
	int iIndexLength;
	int iOffsetLength;

	unsigned long iHitTime;
	unsigned long iMissTime;
	unsigned long iAccCycle;

	//DirectedMapLine* pLine;
	CacheMap*	mpCacheMap;

	//int compare_Tag(ADD64 pTag);
	int getChageSet(int SetNumer);

public:
	int create(int size, int L = 0, int K = 0, int N = 0, enum eChangePolicy eChPolicy = RANDOM, enum eCacheLevel eChLevel = eL1);
	ADD64 Access(int iType, ADD64 pAddress);

	//void SaveResult(int HitTime, int MissTime);
	
	void Print_Setting(void);
	void Print_State(int iPrint_line = 4);
	void Print_Result(void);
	void Save_Result(FILE* pOutLog);
};
