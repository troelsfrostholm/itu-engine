#ifdef __EXP_NMS_UTILITIES
#    define FILE_MANAGEMENT_D __declspec(dllexport)
#else
#    define FILE_MANAGEMENT_D __declspec(dllimport)
#endif

#ifndef NMS_FILEMANAGEMENT
#define NMS_FILEMANAGEMENT


#include <stdio.h>
#include <iostream>

class FILE_MANAGEMENT_D nmsFileManagement
{
	public:
		static long FileSize(FILE *fp);
};
#endif