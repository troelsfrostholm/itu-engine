#include "NMS_Mutex.h"

SDL_mutex* sceneGraphGuard;
SDL_mutex* inputGuard;

void initMutexes()
{
	sceneGraphGuard = SDL_CreateMutex();
	inputGuard = SDL_CreateMutex();
}