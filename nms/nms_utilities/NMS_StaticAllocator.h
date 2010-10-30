#define STATIC_ALLOCATOR_D __declspec(dllexport)

#ifndef NMS_STATIC_ALLOCATOR
#define NMS_STATIC_ALLOCATOR

#include <vector>
#include "NMS_StackBasedAllocator.h"

using namespace std;

#define STATIC_ALLOC StaticAllocator::getInstance()

/*
	StaticMemoryAllocator is a singleton class, which allocates memory that
	doesn't need to be freed until the end of the program. 
*/

class STATIC_ALLOCATOR_D StaticAllocator : public MemoryAllocator
{
public:
	static StaticAllocator * getInstance();
	void * allocMem(size_t size);
	void freeMem(void * pointer);

	U32 currentMemoryUsage(int category);
	void setStackAllocSize(int size);

	~StaticAllocator();

private:
	StaticAllocator();
	
	static int stack_alloc_size;
	static StaticAllocator *instance;
	vector<StackBasedAllocator *> allocators;
};

#endif