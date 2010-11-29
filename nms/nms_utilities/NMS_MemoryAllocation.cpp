#include "NMS_MemoryAllocation.h"
#include <iostream>
#include "NMS_LogFunctions.h"

using namespace std;

//int CurrentMemoryUsage[MEM_NUM_CATEGORIES];
MEMORY_ALLOCATION_D int CurrentMemoryUsage[MEM_NUM_CATEGORIES];

void* operator new (size_t size, MemoryAllocator * allocator, int categoryFlag)
{
	CurrentMemoryUsage[categoryFlag]+=(int) size;
	return allocator->allocMem(size, categoryFlag);
}

void operator delete(void* pMem,  MemoryAllocator * allocator, int categoryFlag)
{
	allocator->freeMem(pMem);
}

 void logMemUsage()
{
/*		LOG.write("Memory usage at end of program : ", LOG_RUN);
	char output[50];
	sprintf(output, "\r\n    Persistent       : %d", CurrentMemoryUsage[MEM_PERSISTENT]);
	LOG.write(output, LOG_RUN);
	sprintf(output, "\r\n    Level            : %d", CurrentMemoryUsage[MEM_LEVEL]);
	LOG.write(output, LOG_RUN);
	sprintf(output, "\r\n    Matrix           : %d", CurrentMemoryUsage[MEM_MATRIX]);
	LOG.write(output, LOG_RUN);
	sprintf(output, "\r\n    Temporary vars   : %d", CurrentMemoryUsage[MEM_TEMP]);
	LOG.write(output, LOG_RUN);*/
}