#include "NMS_StaticAllocator.h"

 int StaticAllocator::stack_alloc_size;
 StaticAllocator *StaticAllocator::instance;

StaticAllocator::StaticAllocator()
{
	stack_alloc_size = 10000;
//	allocators.push_back(new StackBasedAllocator(stack_alloc_size));
}

StaticAllocator::~StaticAllocator()
{
	for(int i=0; i<allocators.size(); i++) {
		delete allocators.at(i);
		allocators.at(i) = NULL;
	}
	instance = NULL;
}

StaticAllocator * StaticAllocator::getInstance()
{
	if(!instance)
		instance = new StaticAllocator();
	return instance;
}

void * StaticAllocator::allocMem(size_t size, int categoryFlag)
{
	if(allocators.size()<=0) {
		allocators.push_back(new StackBasedAllocator(stack_alloc_size));
	}
	if(size > stack_alloc_size) {
		allocators.push_back(new StackBasedAllocator(size));
		return allocators.back()->allocMem(size, categoryFlag);
	}

	try {
		return allocators.back()->allocMem(size, categoryFlag);
	}
	catch(char* msg) {
		allocators.push_back(new StackBasedAllocator(stack_alloc_size));
		return allocators.back()->allocMem(size, categoryFlag);
	}
}

void StaticAllocator::freeMem(void * pointer)
{
}

U32 StaticAllocator::currentMemoryUsage(int category)
{
	return 0;
}

void StaticAllocator::setStackAllocSize(int size)
{
	stack_alloc_size = size;
}