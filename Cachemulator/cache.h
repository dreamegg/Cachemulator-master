#pragma once
struct DirectedMapLine
{
	bool bValid;
	bool bDirty;
	long pTag;
	long pIndex;
	long cOffest;
};


class cache {
private:
	int Type;
	int AssociateNumber;
	
	DirectedMapLine pMap[32];

public:
	int create(int size);
	int init();
	int Access(int iType, unsigned long pAddress);
};