#include <stdio.h>
#include "cache.h"

#define MACHINE_BIT			64

#define Data_read			0
#define Data_write			1
#define Instruction_read	2



int main()
{

	cache MyCache;
	MyCache.create(32);

	int type;
	unsigned long addr;

	FILE *pTrace;
	fopen_s(&pTrace, "trace.txt","r");

	for (int i = 0; i < 10; i++) {
		
		fscanf_s(pTrace, "%d %x\n", &type, &addr);

		printf("%d %x", type, addr);
		MyCache.Access(type, addr);
		printf("\n");
	}
	fclose(pTrace);
	getchar();

	return 0;
}