//#define LEVEL_ALLOCATOR_D __declspec(dllexport)

#ifdef __EXP_NMS_UTILITIES
#    define LEVEL_ALLOCATOR_D __declspec(dllexport)
#else
#    define LEVEL_ALLOCATOR_D __declspec(dllimport)
#endif

#ifndef NMS_LEVEL_ALLOCATOR
#define NMS_LEVEL_ALLOCATOR

#include <vector>
#include "NMS_StackBasedAllocator.h"

using namespace std;

#define LEVEL_ALLOC LevelAllocator::getInstance()

/*
	LevelMemoryAllocator is a singleton class, which allocates memory that
	doesn't need to be freed until the end of the program. 
*/

class LEVEL_ALLOCATOR_D LevelAllocator : public MemoryAllocator
{
public:
	static LevelAllocator * getInstance();
	void * allocMem(size_t size, int categoryFlag);
	void freeMem(void * pointer);
	void clearLevel();

	U32 currentMemoryUsage(int category);
	void setStackAllocSize(int size);

	~LevelAllocator();

private:
	LevelAllocator();
	
	static int stack_alloc_size;
	static LevelAllocator *instance;
	StackBasedAllocator * allocator;
};

#endif