#include "NMS_StubAllocator.h"

StubAllocator * StubAllocator::instance;

StubAllocator::StubAllocator()
{
}

StubAllocator::~StubAllocator()
{
	instance = NULL;
}

StubAllocator * StubAllocator::getInstance()
{
	if(!instance)
		instance = new StubAllocator();
	return instance;
}

void * StubAllocator::allocMem(size_t size)
{
	return malloc(size);
}

void StubAllocator::freeMem(void * pointer)
{
	free(pointer);
	pointer = NULL;
}

U32 StubAllocator::currentMemoryUsage(int category)
{
	return 0;
}