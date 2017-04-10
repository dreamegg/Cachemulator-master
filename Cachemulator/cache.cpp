#include "cache.h"
#include <cmath>
#include <memory.h>
#include <stdio.h>

#define DEBUG	printf
#define MACHINE_BIT			32

long BitMaskUtill(unsigned long pSource, int start, int end)
{
	unsigned long pMask = 0;
	for (int i = 0; i < start - end; i++) {
		pMask = pMask | (1<<i);
	}
	pMask = pMask << end;

	//DEBUG("\nMask   : %x \n", pMask);
	//DEBUG("source : %x \n", pSource);
	//DEBUG("Out    : %x \n\n", pMask & pSource);
	return ((pMask & pSource) >> end);
}

int cache::compare_Tag(unsigned long pTag)
{
	int iReturnCode = ERROR;
	for (int i = 0; i < iLineNumbers; i++)
	{
		if (pTag == pMap[i].pTag)
		{
			printf("Hit at %d", i);
			return i;
		}
	}
	return iReturnCode;
}

int cache::create(int size)
{
	iLine = 0;
	iCacheSize = 8*1024;
	iBlockSize = 16;
	iLineNumbers = iCacheSize / iBlockSize;
	iIndexLength = log2(iLineNumbers);
	iOffsetLength = log2(iBlockSize);
	iTagLength = MACHINE_BIT - iIndexLength - iOffsetLength;

	pMap = new DirectedMapLine[iLineNumbers];
	memset(pMap, 0, sizeof(DirectedMapLine)*iLineNumbers);

	Print_Setting();
	return 0;
}

int cache::Access(int iType, unsigned long pAddress)
{
	int iReturnCode = ERROR;

	unsigned long pSourceTag = BitMaskUtill(pAddress, MACHINE_BIT, iOffsetLength + iIndexLength);
	switch (iType)
	{
	case DATA_READ:
	case DATA_WRITE:
	case INST_READ:
		if (compare_Tag(pSourceTag) == ERROR)
		{
			pMap[iLine].bValid = true;
			pMap[iLine].pTag = BitMaskUtill(pAddress, MACHINE_BIT, iOffsetLength + iIndexLength);
			pMap[iLine].pIndex = BitMaskUtill(pAddress, iOffsetLength + iIndexLength, iOffsetLength);
			pMap[iLine].cOffest = BitMaskUtill(pAddress, iOffsetLength, 0);

			printf("\nLine %d : %10x %10x %10x  == MISSS", iLine, pMap[iLine].pTag, pMap[iLine].pIndex, pMap[iLine].cOffest);
			iReturnCode = MISS;
		}
		else
		{
			iReturnCode = HIT;
		}
		
		break;
	default:
		break;
	}
	return iReturnCode;
}

void cache::Print_Setting(void)
{
	printf("======<Cach is created as...>==========\n");
	printf("iMainMemorySize : %8d		iCacheSize:%d\n", iMainMemorySize, iCacheSize);
	printf("iBlockSize: %8d		iLineNumbers:%8d\n", iBlockSize, iLineNumbers);
}
