#include "NMS_StaticAllocator.h"
/*
StaticAllocator::StaticAllocator()
{
	stack_alloc_size = 10;
	allocators.push_back(StackBasedAllocator(stack_alloc_size));
}

StaticAllocator::~StaticAllocator()
{
	delete instance;
	instance = NULL;
}

StaticAllocator * StaticAllocator::getInstance()
{
	if(!instance)
		instance = new StaticAllocator();
	return instance;
}

void * StaticAllocator::allocMem(size_t size)
{
	if(size > stack_alloc_size) {
		allocators.push_back(StackBasedAllocator(size));
		return allocators.back().allocMem(size);
	}

	try {
		return allocators.back().allocMem(size);
	}
	catch(char* msg) {
		allocators.push_back(StackBasedAllocator(stack_alloc_size));
		return allocators.back().allocMem(size);
	}
}

void StaticAllocator::freeMem(void * pointer)
{
}

U32 StaticAllocator::currentMemoryUsage(int category)
{
	return 0;
}*/