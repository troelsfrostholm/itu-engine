//#define MEMORY_ALLOCATION_D __declspec(dllexport)

#ifdef __EXP_NMS_UTILITIES
#    define MEMORY_ALLOCATION_D __declspec(dllexport)
#else
#    define MEMORY_ALLOCATION_D __declspec(dllimport)
#endif

#ifndef NMS_MEMORY_ALLOCATION
#define NMS_MEMORY_ALLOCATION

#include "NMS_CustomTypes.h"
#include "NMS_MemoryTracker.h"

typedef enum {
	MEM_PERSISTENT,
	MEM_LEVEL,
	MEM_VERTEX,
	MEM_TEMPORARY } memCategories;

class MEMORY_ALLOCATION_D MemoryAllocator
{
public:
	virtual void * allocMem(size_t size) = 0;
	virtual void freeMem(void * pointer) = 0;

	virtual U32 currentMemoryUsage(int category) = 0;
};

MEMORY_ALLOCATION_D void * operator new (size_t size, MemoryAllocator * allocator, int categoryFlag);
MEMORY_ALLOCATION_D void operator delete(void* pMem,  MemoryAllocator * allocator, int categoryFlag);

#endif