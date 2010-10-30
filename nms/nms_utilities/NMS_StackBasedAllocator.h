#define STACK_ALLOCATOR_D __declspec(dllexport)

#ifndef NMS_STACK_ALLOCATOR
#define NMS_STACK_ALLOCATOR

#include <cstdlib>
#include <iostream>
#include "NMS_MemoryAllocation.h"

using namespace std;

class STACK_ALLOCATOR_D StackBasedAllocator : public MemoryAllocator
{
protected:
	size_t allocator_size; //Size of allocator
	U32 Begin;             //Memory address where allocated objects begin
	U32 Marker;	           //Memory address of next point to start allocation
public:
	StackBasedAllocator();
	StackBasedAllocator(unsigned int size_in_bytes);
	~StackBasedAllocator();
	void * allocMem(size_t size);
	void freeMem(void * ptr);
	void * allocUnaligned(size_t size_in_bytes);
	void * allocAligned(size_t size_in_bytes, U32 alignment);
	U32 calculateAllignedAddress(U32 raw_address, U32 alignment);
	void clear();
	U32 getMarker();
	void freeToMarker(U32 marker);
	U32 currentMemoryUsage(int category);
};

#endif