#include "nms_watcher.h"

nms_watcher::nms_watcher()
{
	path = "../scripts/";
	extPath = "../scripts/*js";
	scripts = std::vector<std::string>();
}

nms_watcher::~nms_watcher()
{

}

char* nms_watcher::getPath()
{
	return path;
}

std::vector<std::string> nms_watcher::getScripts()
{
	return scripts;
}

void nms_watcher::scanFiles()
{
	scripts.clear();
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(extPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
      printf ("FindFirstFile failed (%d)\n", GetLastError());
      return;
	} 
	else 
	{
		scripts.push_back(std::string(FindFileData.cFileName));
	}
	while(FindNextFile(hFind, &FindFileData))
	{
		scripts.push_back(std::string(FindFileData.cFileName));
	}
	FindClose(hFind);
}