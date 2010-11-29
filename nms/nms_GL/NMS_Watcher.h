#ifdef __EXP_NMS_GL
#    define WATCHER_D __declspec(dllexport)
#else
#    define WATCHER_D __declspec(dllimport)
#endif

#ifndef NMS_WATCHER_H__
#define NMS_WATCHER_H__

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <vector>
#include "string.h"

class WATCHER_D nms_watcher
{
	private:
		char* path;
		char* extPath;
		std::vector<std::string> scripts;
	public:
		nms_watcher();
		~nms_watcher();
		std::vector<std::string> getScripts();
		char* getPath();
		void scanFiles();
};
#endif;