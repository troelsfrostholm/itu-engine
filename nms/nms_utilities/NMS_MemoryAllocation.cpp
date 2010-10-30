#include "NMS_MemoryAllocation.h"
#include <iostream>

using namespace std;

void* operator new (size_t size, MemoryAllocator * allocator, int categoryFlag)
{
	return allocator->allocMem(size);
}