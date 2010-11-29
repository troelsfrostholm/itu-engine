#ifdef __EXP_NMS_GL
#    define ENGINE_D __declspec(dllexport)
#else
#    define ENGINE_D __declspec(dllimport)
#endif

#ifndef NMS_ENGINE_H__
#define NMS_ENGINE_H__

#include "NMS_Framework.h"

extern ENGINE_D NMS_Framework engine;

bool ENGINE_D InitEngine(int width,int height,int bpp,char* windowTitle,bool fullscreen);

void ENGINE_D StartEngine();

void ENGINE_D QuitEngine();

#endif;