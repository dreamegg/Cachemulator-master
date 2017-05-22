#include <stdio.h>
#include <string>
#include "cache.h"

#define MAX_TEST_COUNT	400

struct Total_Result
{
	long HitCount;
	long MissCount;
	long MissPanalty;	
};

int main()
{
	cache MyCacheInstL1[MAX_TEST_COUNT];
	cache MyCacheDataL1[MAX_TEST_COUNT];
	cache MyCacheL2[MAX_TEST_COUNT];
	cache MyCacheL3[MAX_TEST_COUNT];
	struct Total_Result stResult[MAX_TEST_COUNT] = { 0, };
	
	//-N = 1 case (fully associative mapping): LK = 64KB, 256KB, 512KB(variables : L = 64, 128bytes)
	int testCount = 0;
#if 0
	for (int i = 64; i <= 1024; i = i * 2)
	{
		for (int j = 64; j <= 1024; j = j * 2)
		{
			MyCache[testCount].create(i * 1024, j, 0, 1, FIFO);		//FA, L=64,LK=64
			testCount++;
		}
	}

	// -K=1 case (direct mapping): LN= 64, 256, 512KB (variable: L= 64bytes)
	for (int i = 64; i <= 1024; i = i * 2)
	{
		for (int j = 64; j <= 1024; j = j * 2)
		{
			MyCache[testCount].create(i * 1024, j, 1);
			testCount++;
		}
	}

	//-K = 2, 4 ,8 case (set associative mapping, L = 64, 128byte
	for (int i = 64; i <= 1024; i = i * 2)
	{
		for (int j = 64; j <= 1024; j = j * 2)
		{
			//MyCache[testCount].create(i * 1024, j, 2, 0, FIFO);
			//testCount++;
			//MyCache[testCount].create(i * 1024, j, 4, 0, FIFO);
			//testCount++;
			MyCache[testCount].create(i * 1024, j, 8, 0, FIFO);
			testCount++;
		}
	}
#endif
	MyCacheInstL1[testCount].create(32 * 1024, 64, 2, 0, FIFO);
	MyCacheDataL1[testCount].create(32 * 1024, 64, 2, 0, FIFO);
	MyCacheL2[testCount].create(256 * 1024, 64, 8, 0, FIFO, eL2);
	testCount++;

	MyCacheInstL1[testCount].create(32 * 1024, 64, 2, 0, FIFO);
	MyCacheDataL1[testCount].create(32 * 1024, 64, 2, 0, FIFO);
	MyCacheL2[testCount].create(512 * 1024, 64, 8, 0, FIFO, eL2);
	testCount++;

	MyCacheInstL1[testCount].create(32 * 1024, 64, 2, 0, LRU);
	MyCacheDataL1[testCount].create(32 * 1024, 64, 2, 0, LRU);
	MyCacheL2[testCount].create(256 * 1024, 64, 8, 0, LRU, eL2);
	testCount++;

	MyCacheInstL1[testCount].create(32 * 1024, 64, 2, 0, LRU);
	MyCacheDataL1[testCount].create(32 * 1024, 64, 2, 0, LRU);
	MyCacheL2[testCount].create(512 * 1024, 64, 8, 0, LRU, eL2);
	testCount++;

	FILE *pTrace;
	int type;
	ADD64 addr;

	fopen_s(&pTrace, "bzip2", "r");

	for (;;) {
	//for (long i = 0; i < 3000000; i++) {
		if (fscanf_s(pTrace, "%d %llx\n", &type, &addr) == EOF) break;
		for (int n = 0; n < testCount; n++)
		{
			stResult[n].HitCount++;
			int bReturn = MISS;
			switch (type)
			{
			case DATA_READ:
			case DATA_WRITE:
				bReturn = MyCacheDataL1[n].Access(type, addr);
				break;
			case INST_READ:
				bReturn = MyCacheInstL1[n].Access(type, addr);
				break;
			default:
				break;
			}

			if (bReturn != HIT)
			{
				bReturn = MyCacheL2[n].Access(type, bReturn);
				if (bReturn != HIT) stResult[n].MissCount++;
			}
		}
	}
	fclose(pTrace);

	FILE *pOutLog;
	fopen_s(&pOutLog, "out_astar.txt", "w");
	for (int n = 0; n < testCount; n++)
	{
		printf("\n========[ L1 Inst Cache :[%d]=======\n", n);
		MyCacheInstL1[n].Print_Setting();
		MyCacheInstL1[n].Print_Result();
		printf("\n========[ L1 Data Cache :[%d]=======\n", n);
		MyCacheDataL1[n].Print_Setting();
		MyCacheDataL1[n].Print_Result();
		printf("\n========[ L2 Cache :[%d]=======\n", n);
		MyCacheL2[n].Print_Setting();
		MyCacheL2[n].Print_Result();
		//MyCache[n].Save_Result(pOutLog);
		printf("\n========[ iTotalMiss :[%d / %d]=======\n", stResult[n].MissCount, stResult[n].HitCount);
		
	}
	
	fclose(pOutLog);

	//============================================
#if 0
	//-N = 1 case (fully associative mapping): LK = 64KB, 256KB, 512KB(variables : L = 64, 128bytes)
	testCount = 0;
	for (int i = 64; i <= 1024; i = i * 2)
	{
		for (int j = 64; j <= 1024; j = j * 2)
		{
			MyCache[testCount].create(i * 1024, j, 0, 1, FIFO);		//FA, L=64,LK=64
			testCount++;
		}
	}

	// -K=1 case (direct mapping): LN= 64, 256, 512KB (variable: L= 64bytes)
	for (int i = 64; i <= 1024; i = i * 2)
	{
		for (int j = 64; j <= 1024; j = j * 2)
		{
			MyCache[testCount].create(i * 1024, j, 1);
			testCount++;
		}
	}
	//-K = 2, 4 ,8 case (set associative mapping, L = 64, 128byte
	for (int i = 64; i <= 1024; i = i * 2)
	{
		for (int j = 64; j <= 1024; j = j * 2)
		{
			MyCache[testCount].create(i * 1024, j, 2, 0, FIFO);
			testCount++;
			MyCache[testCount].create(i * 1024, j, 4, 0, FIFO);
			testCount++;
			MyCache[testCount].create(i * 1024, j, 8, 0, FIFO);
			testCount++;
		}
	}
	for (int i = 64; i <= 1024; i = i * 2)
	{
		for (int j = 64; j <= 1024; j = j * 2)
		{
			MyCache[testCount].create(i * 1024, j, 2, 0, PSUEDO);
			testCount++;
			MyCache[testCount].create(i * 1024, j, 2, 0, FIFO);
			testCount++;
		}
	}

	fopen_s(&pTrace, "bzip2", "r");
	for (;;) {
	//for (long i = 0; i < 300000; i++) {
		if (fscanf_s(pTrace, "%d %llx\n", &type, &addr) == EOF) break;
		for (int n = 0; n < testCount; n++)
		{
			MyCache[n].Access(type, addr);
		}
	}
	fclose(pTrace);

	fopen_s(&pOutLog, "out_bzip2.txt", "w");
	for (int n = 0; n < testCount; n++)
	{
		printf("\n========[ Cache :[%d]=======\n", n);
		MyCache[n].Print_Setting();
		MyCache[n].Print_Result();
		MyCache[n].Save_Result(pOutLog);
	}
	fclose(pOutLog);
#endif
	getchar();
	
	return 0;
}
