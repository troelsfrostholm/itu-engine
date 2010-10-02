#ifdef __EXP_NMS_GL
#    define THREAD_D __declspec(dllexport)
#else
#    define THREAD_D __declspec(dllimport)
#endif

#ifndef NMS_THREAD_H__
#define NMS_THREAD_H__

#include "SDL.h"
#include <iostream>
#include <assert.h>

using namespace std;

int THREAD_D threadProxy( void *pointer );

class THREAD_D Thread
{
private:
	SDL_Thread *thread;

public:
	Thread();
	void start();
	SDL_Thread* getThread();

	virtual int run() = 0;
};

#endif