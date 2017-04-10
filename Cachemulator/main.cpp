#include <stdio.h>
#include "cache.h"

int main()
{

	cache MyCache;
	MyCache.create(32);

	int type;
	unsigned long addr;

	int iHitCount = 0;
	int iMissCount = 0;

	FILE *pTrace;
	fopen_s(&pTrace, "trace.txt","r");

	int i;

	for (i = 0; i < 100 ; i++) {
		
		if (fscanf_s(pTrace, "%d %x\n", &type, &addr) == EOF) break;

		if (MyCache.Access(type, addr) == HIT)	iHitCount++;
		else iMissCount++;
		printf("\n");
	}
	fclose(pTrace);

	printf("Total Acc Time :%4d Hit Count : %4d, Miss Count : %4d \n",i, iHitCount, iMissCount);
	getchar();

	return 0;
}