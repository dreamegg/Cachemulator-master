#pragma once
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

	
	DirectedMapLine* pMap;

public:
	int create(int size);
	int init();
	int Access(int iType, unsigned long pAddress);
};