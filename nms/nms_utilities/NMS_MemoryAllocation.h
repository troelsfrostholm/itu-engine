#define MEMORY_ALLOCATION_D __declspec(dllexport)

#ifndef NMS_MEMORY_ALLOCATION
#define NMS_MEMORY_ALLOCATION

#include "NMS_CustomTypes.h"
#include "NMS_MemoryTracker.h"

class MEMORY_ALLOCATION_D MemoryAllocator
{
public:
	static const int NUM_CATEGORIES = 2;

	virtual void * allocMem(size_t size) = 0;
	virtual void freeMem(void * pointer) = 0;

	virtual U32 currentMemoryUsage(int category) = 0;
};

MEMORY_ALLOCATION_D void * operator new (size_t size, MemoryAllocator * allocator, int categoryFlag);

#endif