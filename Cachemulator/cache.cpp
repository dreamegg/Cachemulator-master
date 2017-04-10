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

	DEBUG("\nMask   : %x \n", pMask);
	DEBUG("source : %x \n", pSource);
	DEBUG("Out    : %x \n\n", pMask & pSource);
	return ((pMask & pSource) >> end);
}

int cache::create(int size)
{
	iCacheSize = 8*1024;
	iBlockSize = 16;
	iLineNumbers = iCacheSize / iBlockSize;
	iIndexLength = log2(iLineNumbers);
	iOffsetLength = log2(iBlockSize);
	iTagLength = MACHINE_BIT - iIndexLength - iOffsetLength;

	pMap = new DirectedMapLine[iLineNumbers];
	memset(pMap, 0, sizeof(DirectedMapLine)*iLineNumbers);

	return 0;
}

int cache::Access(int iType, unsigned long pAddress)
{
	printf("Tag   : %32x", BitMaskUtill(pAddress, MACHINE_BIT, iOffsetLength + iIndexLength));
	printf("Tag   : %32x", BitMaskUtill(pAddress, iOffsetLength+iIndexLength, iOffsetLength));
	printf("Tag   : %32x \n", BitMaskUtill(pAddress, iOffsetLength, 0));
	return 0;
}