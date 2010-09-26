#ifndef NMS_FILEMANAGEMENT
#define NMS_FILEMANAGEMENT

#include <stdio.h>
#include <iostream>

class __declspec(dllexport) nmsFileManagement
{
	public:
		static long FileSize(FILE *fp);
};
#endif // SHA1_DEFINED