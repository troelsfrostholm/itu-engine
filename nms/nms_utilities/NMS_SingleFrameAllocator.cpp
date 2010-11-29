#include "NMS_SingleframeAllocator.h"

int SingleframeAllocator::stack_alloc_size;
SingleframeAllocator *SingleframeAllocator::instance;

SingleframeAllocator::SingleframeAllocator()
{
	stack_alloc_size = 100000000;
	allocator = new StackBasedAllocator(stack_alloc_size);
//	allocators.push_back(new StackBasedAllocator(stack_alloc_size));
}

SingleframeAllocator::~SingleframeAllocator()
{
	delete allocator;
	allocator = NULL;
	instance = NULL;
}

SingleframeAllocator * SingleframeAllocator::getInstance()
{
	if(!instance)
		instance = new SingleframeAllocator();
	return instance;
}

void * SingleframeAllocator::allocMem(size_t size, int categoryFlag)
{
	if(!allocator) {
		allocator = new StackBasedAllocator(stack_alloc_size);
	}
	return allocator->allocMem(size, categoryFlag);
}

void SingleframeAllocator::freeMem(void * pointer)
{
}

void SingleframeAllocator::clear()
{
	allocator->clear();
}

U32 SingleframeAllocator::currentMemoryUsage(int category)
{
	return 0;
}

void SingleframeAllocator::setStackAllocSize(int size)
{
	stack_alloc_size = size;
}