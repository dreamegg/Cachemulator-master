#include "cache.h"
#include <stdio.h>

#define DEBUG	printf

long BitMaskUtill(unsigned long pSource, int start, int end)
{
	unsigned long pMask = 0;
	for (int i = 0; i < start+1 - end; i++) {
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
	return 0;
}

int cache::Access(int iType, unsigned long pAddress)
{
	printf("Tag   : %32x", BitMaskUtill(pAddress, 31, 13));
	printf("Tag   : %32x", BitMaskUtill(pAddress, 12, 4));
	printf("Tag   : %32x \n", BitMaskUtill(pAddress, 3, 0));
	return 0;
}