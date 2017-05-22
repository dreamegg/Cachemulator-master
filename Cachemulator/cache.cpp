#include "cache.h"
#include <cmath>
#include <memory.h>

#define MACHINE_BIT			64

ADD64 BitMaskUtill(ADD64 pSource, int start, int end)
{
	ADD64 pMask = 0;
	for (int i = 0; i < start - end; i++) {
		pMask = pMask | (1<<i);
	}
	pMask = pMask << end;
	return ((pMask & pSource) >> end);
}

int cache::getChageSet(int SetNumer)
{
	int iReturnSet = -1;

	for (int i = 0; i < AssociateNumber; i++)	// Find Empty line
	{
		if (mpCacheMap[i].pBlocks[iLinePosition].bValid == false)
		{
			return i;
		}
	}

	if (AssociateNumber == 1)	//DM
	{
		return 0;
	}

	switch (ePolicy)
	{
	case RANDOM:
		iReturnSet = rand() % AssociateNumber;
		break;
	case FIFO:
		iReturnSet = iChangePosition;
		iChangePosition = iChangePosition + 1;
		iChangePosition = iChangePosition% AssociateNumber;
		break;
	case LRU:
		int i;
		for (i = 0; i < AssociateNumber; i++)
		{
			if (mpCacheMap[i].iAssoIndex == (AssociateNumber - 1))
			{
				for (int itr = 0; itr < AssociateNumber; itr++)
				{
					if (mpCacheMap[i].iAssoIndex > mpCacheMap[itr].iAssoIndex)
						mpCacheMap[itr].iAssoIndex++;
				}
				mpCacheMap[i].iAssoIndex = 0;
				//printf("[%d] %d : %d : %d : %d\n", iHitTime, mpCacheMap[0].iAssoIndex, mpCacheMap[1].iAssoIndex, mpCacheMap[2].iAssoIndex, mpCacheMap[3].iAssoIndex);
				break;
			}
		}
		iReturnSet = i;
		break;
	case PSUEDO:
		iReturnSet = 0;
		break;
	}

	return iReturnSet;
}

int cache::create(int size,int L, int K, int N, enum eChangePolicy eChPolicy, enum eCacheLevel eChLevel)
{
	iCacheSize = size;		//LK
	if (K == 0)					//Full Asso
	{
		AssociateNumber = size / L;
		iIndexLength = 0;
	}
	else 
	{
		AssociateNumber = K;	//if K=1, Directed map
		if (N == 0)
			N = size / K / L;
		iIndexLength = log2(N);
	}
	iLineNumbers = N;
	iLinePosition = 0;

	ePolicy = eChPolicy;
	iChangePosition = 0;

	if (eChLevel == eL1)	iMissPanelty = L1_MISS_PENALTY;
	else if (eChLevel == eL2)	iMissPanelty = L2_MISS_PENALTY;
		 
	iMissTime = 0;
	iHitTime = 0;
	iAccCycle = 0;

	iBlockSize = L;
	
	iOffsetLength = log2(iBlockSize);
	iTagLength = MACHINE_BIT - iIndexLength - iOffsetLength;

	mpCacheMap = new CacheMap[AssociateNumber];
	memset(mpCacheMap, 0, sizeof(CacheMap)*AssociateNumber);

	for (int i = 0; i < AssociateNumber; i++)
	{
		mpCacheMap[i].pBlocks = new DirectedMapLine[iLineNumbers];
		memset(mpCacheMap[i].pBlocks, 0, sizeof(DirectedMapLine)*iLineNumbers);
		for (int j = 0; j < iLineNumbers; j++)
		{
			mpCacheMap[i].pBlocks[j].bValid = false;
			mpCacheMap[i].pBlocks[j].pTag = 0;
		}
		mpCacheMap[i].iAssoIndex = i;
		mpCacheMap[i].bDirty = false;
	}
	Print_Setting();
	return 0;
}

ADD64 cache::Access(int iType, ADD64 pAddress)
{
	int iReturnCode = ERROR;
	int i;
	int iHitPosition = 0;

	ADD64 pSourceTag = (pAddress >> (iIndexLength + iOffsetLength));
	iLinePosition = ((pAddress << iTagLength) >> (iTagLength + iOffsetLength));

	switch (iType)
	{
	case DATA_READ:
	case DATA_WRITE:
	case INST_READ:
		for(i = 0; i < AssociateNumber; i++)
		{
			if (mpCacheMap[i].pBlocks[iLinePosition].bValid == false) {
				iHitPosition = i;
			}
			if ((mpCacheMap[i].pBlocks[iLinePosition].bValid == true) && (mpCacheMap[i].pBlocks[iLinePosition].pTag == pSourceTag)) {
				iHitTime++;
				iReturnCode = HIT;
				if (ePolicy == LRU)
				{
					for (int itr = 0; itr < AssociateNumber; itr++)
					{
						if (mpCacheMap[i].iAssoIndex > mpCacheMap[itr].iAssoIndex) 
							mpCacheMap[itr].iAssoIndex++;
					}
					mpCacheMap[i].iAssoIndex = 0;
				}
				break;
			}
			iAccCycle = iAccCycle + COMPARE_PENALTY;
		}

		if (ePolicy == PSUEDO)
		{
			//H1 Miss H2 Hit
			if (iReturnCode == HIT && iHitPosition == 1)
			{
				//swap cache line
				ADD64 pTempTag = mpCacheMap[0].pBlocks[iLinePosition].pTag;
				mpCacheMap[0].pBlocks[iLinePosition].pTag = mpCacheMap[1].pBlocks[iLinePosition].pTag;
				mpCacheMap[1].pBlocks[iLinePosition].pTag = pTempTag;

				bool bTemp = mpCacheMap[0].pBlocks[iLinePosition].bValid;
				mpCacheMap[0].pBlocks[iLinePosition].bValid = mpCacheMap[1].pBlocks[iLinePosition].bValid;
				mpCacheMap[1].pBlocks[iLinePosition].bValid = bTemp;
			}
		}

		if (i == AssociateNumber)	// Cache Miss!
		{
			iHitPosition = getChageSet(iHitPosition);
			
			ADD64 reCreateAddress = pAddress >> (iIndexLength + iOffsetLength) << (iIndexLength + iOffsetLength);
			reCreateAddress = reCreateAddress | mpCacheMap[iHitPosition].pBlocks[iLinePosition].pTag;
			iReturnCode = reCreateAddress;
			
			mpCacheMap[iHitPosition].pBlocks[iLinePosition].pTag = pSourceTag;
			mpCacheMap[iHitPosition].pBlocks[iLinePosition].bValid = true;
			iMissTime++;
			iAccCycle = iAccCycle + iMissPanelty;
			
		}
		break;
	default:
		break;
	}
	return iReturnCode;
}

void cache::Print_Setting(void)
{
	//printf("\n======<Cach is defined as...>==========\n");
	printf("iCacheSize[L] : %d	Associativity [K]: %d	Line [N]:%d \n", iCacheSize, AssociateNumber, iLineNumbers);
	printf("Type is : ");
	if (AssociateNumber == 1)	printf("Directed Map\n");
	else if (AssociateNumber != 1 && (iCacheSize / iBlockSize*AssociateNumber) != 1) printf("%d Way Associativity\n", AssociateNumber);
	else printf("Fully Associativity\n");
	printf("<<----Tag [%d] --->|<--- Index [%d] ---->|<----Offset[%d]---->\n", iTagLength, iIndexLength, iOffsetLength);
}

void cache::Print_State(int iPrint_line)
{
	system("cls");
	Print_Setting();
	for (int i = 0; i < AssociateNumber && i< iPrint_line ; i++)
	{
		printf("============== Set [%4d] =======================\n", i);
		printf("[Line][	VB]	[TAG]	[Index]	[Offset]\n");
		for (int j = 0; j < iPrint_line; j++)
		{
			printf("%4d : %2x %8x ====data====\n", j, mpCacheMap[i].pBlocks[j].bValid, mpCacheMap[i].pBlocks[j].pTag,j);
		}
		printf("...\n...\n...\n");
		
	}
}

void cache::Print_Result(void)
{
	long lTotalAcc = iHitTime + iMissTime;
	float lAMAT = (iHitTime + iAccCycle) / lTotalAcc;

	printf("Total Acc Time :%4d Hit Count : %4d, Miss Count : %4d  \nMiss Rate : [%d] \n\n", lTotalAcc, iHitTime, iMissTime, iMissTime * 1000 / (lTotalAcc));
	printf("	%d	%d	%d	%d	%d	%d	%5.5d	%8d \n", iCacheSize / 1024, iBlockSize, AssociateNumber, lTotalAcc, iHitTime, iMissTime, lAMAT, iMissTime * 1000 / lTotalAcc);
}

void cache::Save_Result(FILE* pOutLog)
{
	long lTotalAcc = iHitTime + iMissTime;
	long lAMAT = (iHitTime + iAccCycle) / lTotalAcc;
	
	if (AssociateNumber == 1)	fprintf(pOutLog, "DM");
	else if (AssociateNumber ==2 && ePolicy == PSUEDO ) fprintf(pOutLog, "PSUEDO", AssociateNumber);
	else if (AssociateNumber > 1 && AssociateNumber < 16) fprintf(pOutLog, "%d Way", AssociateNumber);
	else fprintf(pOutLog, "FA");
	fprintf(pOutLog, "	%d	%d	%d	%d	%d	%d	%d	%8d \n", iCacheSize/1024,iBlockSize, AssociateNumber, lTotalAcc , iHitTime, iMissTime, lAMAT, iMissTime * 1000 / lTotalAcc);
}

