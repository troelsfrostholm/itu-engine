//#define SINGLEFRAME_ALLOCATOR_D __declspec(dllexport)

#ifdef __EXP_NMS_UTILITIES
#    define SINGLEFRAME_ALLOCATOR_D __declspec(dllexport)
#else
#    define SINGLEFRAME_ALLOCATOR_D __declspec(dllimport)
#endif

#ifndef NMS_SINGLEFRAME_ALLOCATOR
#define NMS_SINGLEFRAME_ALLOCATOR

#include <vector>
#include "NMS_StackBasedAllocator.h"

using namespace std;

#define SINGLEFRAME_ALLOC SingleframeAllocator::getInstance()

/*
	SingleframeMemoryAllocator is a singleton class, which allocates memory that
	doesn't need to be freed until the end of the program. 
*/

class SINGLEFRAME_ALLOCATOR_D SingleframeAllocator : public MemoryAllocator
{
public:
	static SingleframeAllocator * getInstance();
	void * allocMem(size_t size);
	void freeMem(void * pointer);
	void clear();

	U32 currentMemoryUsage(int category);
	void setStackAllocSize(int size);

	~SingleframeAllocator();

private:
	SingleframeAllocator();
	
	static int stack_alloc_size;
	static SingleframeAllocator *instance;
	StackBasedAllocator * allocator;
};

#endif