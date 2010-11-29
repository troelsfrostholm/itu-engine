#include "NMS_LevelAllocator.h"

 int LevelAllocator::stack_alloc_size;
 LevelAllocator *LevelAllocator::instance;

LevelAllocator::LevelAllocator()
{
	stack_alloc_size = 100000000;
	allocator = NULL;
//	allocators.push_back(new StackBasedAllocator(stack_alloc_size));
}

LevelAllocator::~LevelAllocator()
{
	delete allocator;
	allocator = NULL;
	instance = NULL;
}

LevelAllocator * LevelAllocator::getInstance()
{
	if(!instance)
		instance = new LevelAllocator();
	return instance;
}

void * LevelAllocator::allocMem(size_t size, int categoryFlag)
{
	if(!allocator) {
		allocator = new StackBasedAllocator(stack_alloc_size);
	}
	return allocator->allocMem(size, categoryFlag);
}

void LevelAllocator::freeMem(void * pointer)
{
}

void LevelAllocator::clearLevel()
{
	allocator->clear();
}

U32 LevelAllocator::currentMemoryUsage(int category)
{
	return 0;
}

void LevelAllocator::setStackAllocSize(int size)
{
	stack_alloc_size = size;
}