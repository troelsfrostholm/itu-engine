#include "NMS_Mutex.h"

SDL_mutex* sceneGraphGuard;

void initMutexes()
{
	sceneGraphGuard = SDL_CreateMutex();
}