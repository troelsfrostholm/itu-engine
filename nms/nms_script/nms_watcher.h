#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <vector>
#include "string.h"

class nms_watcher
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
