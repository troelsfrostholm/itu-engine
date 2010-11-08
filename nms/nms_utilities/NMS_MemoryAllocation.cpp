#include "NMS_MemoryAllocation.h"
#include <iostream>

using namespace std;

void* operator new (size_t size, MemoryAllocator * allocator, int categoryFlag)
{
	//LOG.write("In custom new", LOG_DEBUG);
	CurrentMemoryUsage[categoryFlag]+=(int) size;
	return allocator->allocMem(size);
}

void operator delete(void* pMem,  MemoryAllocator * allocator, int categoryFlag)
{
	allocator->freeMem(pMem);
}