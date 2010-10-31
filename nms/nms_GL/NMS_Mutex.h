#ifdef __EXP_NMS_GL
#    define MUTEX_D __declspec(dllexport)
#else
#    define MUTEX_D __declspec(dllimport)
#endif

#ifndef NMS_MUTEX__
#define NMS_MUTEX__

#include "SDL.h"

extern MUTEX_D SDL_mutex* sceneGraphGuard;
extern MUTEX_D SDL_mutex* inputGuard;

extern MUTEX_D void initMutexes();

#endif