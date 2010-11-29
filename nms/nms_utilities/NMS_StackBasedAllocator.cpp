#include "NMS_StackBasedAllocator.h"

StackBasedAllocator::StackBasedAllocator()
{
	StackBasedAllocator(0);
}

StackBasedAllocator::StackBasedAllocator(unsigned int size_in_bytes)
{
	allocator_size = size_in_bytes;
	Begin = (U32) malloc(size_in_bytes*sizeof(unsigned char));
	clear();
}

StackBasedAllocator::~StackBasedAllocator()
{
	free((void *) Begin);
	Begin = NULL;
}

U32 StackBasedAllocator::calculateAllignedAddress(U32 raw_address, U32 alignment)
{
	U32 mask = (alignment - 1);
	U32 misalignment = raw_address & mask;
	U32 adjustment = alignment - misalignment;
	U32 alignedAddress = raw_address + adjustment;
	return alignedAddress;
}

void * StackBasedAllocator::allocMem(size_t size, int categoryFlag)
{
	CurrentMemoryUsage[categoryFlag]+=(int) size;
	return allocUnaligned(size);
}

void StackBasedAllocator::freeMem(void * ptr)
{
	ptr = NULL;
}

void * StackBasedAllocator::allocAligned(size_t size, U32 alignment)
{
	U32 size_in_bytes = size / sizeof(BYTE);
	U32 alignedAddress = calculateAllignedAddress(Marker, alignment);
	U32 nextAddress = alignedAddress + size_in_bytes;

	//If they don't fit in the allocator, just return null
	if(allocator_size <= nextAddress - Begin)
		return NULL;
	
	//push a pointer to the address to the stack and return it
	Marker = nextAddress;
	return (void *) alignedAddress;
}

void * StackBasedAllocator::allocUnaligned(size_t size)
{
	unsigned int size_in_bytes = size / sizeof(BYTE);
	if(allocator_size < Marker - Begin + size_in_bytes)
		throw "Allocator size exceeded";
	void * StartOfBlock = (void *) Marker;
	Marker += size_in_bytes;
	return StartOfBlock;
}


void StackBasedAllocator::clear()
{
	Marker = Begin;
}

U32 StackBasedAllocator::getMarker()
{
	return Marker;
}

void StackBasedAllocator::freeToMarker(U32 marker)
{
	Marker = marker;
}

U32 StackBasedAllocator::currentMemoryUsage(int category)
{
	return Marker - Begin;
}