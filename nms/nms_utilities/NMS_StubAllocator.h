//#define STUB_ALLOCATOR_D __declspec(dllexport)

#ifdef __EXP_NMS_UTILITIES
#    define STUB_ALLOCATOR_D __declspec(dllexport)
#else
#    define STUB_ALLOCATOR_D __declspec(dllimport)
#endif

#ifndef NMS_STUB_ALLOCATOR
#define NMS_STUB_ALLOCATOR

#include <vector>
#include "NMS_StackBasedAllocator.h"

using namespace std;

#define STUB_ALLOC StubAllocator::getInstance()

/*
	LevelMemoryAllocator is a singleton class, which allocates memory that
	doesn't need to be freed until the end of the program. 
*/

class STUB_ALLOCATOR_D StubAllocator : public MemoryAllocator
{
public:
	static StubAllocator * getInstance();
	void * allocMem(size_t size);
	void freeMem(void * pointer);

	U32 currentMemoryUsage(int category);

	~StubAllocator();

private:
	StubAllocator();
	
	static StubAllocator *instance;
};

#endif