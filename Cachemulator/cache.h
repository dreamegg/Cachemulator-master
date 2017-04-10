#pragma once

#define DATA_READ		0
#define DATA_WRITE		1
#define INST_READ		2

#define HIT			1
#define MISS		0
#define ERROR		-1

struct DirectedMapLine
{
	bool bValid;
	bool bDirty;
	unsigned long pTag;
	unsigned long pIndex;
	unsigned long cOffest;
};


class cache {
private:
	int Type;
	int AssociateNumber;

	int iMainMemorySize;
	int iCacheSize;
	int iBlockSize;
	int iLineNumbers;

	int iWriteType;

	int iTagLength;
	int iIndexLength;
	int iOffsetLength;

	int iLine;

	DirectedMapLine* pMap;

	int compare_Tag(unsigned long pTag);

public:
	int create(int size);
	int init();
	int Access(int iType, unsigned long pAddress);
	void Print_Setting(void);
};