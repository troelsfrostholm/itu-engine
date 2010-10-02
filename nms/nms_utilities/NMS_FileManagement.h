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
		static long  FileSize(FILE *fp);
		static bool  FileExists(const char * filename);
		static void* Malloc(size_t size);                                    //Allocate the memory space with regard to the type to be used. Return a pointer to the memory allocation
		static void	 Free(void** p);                                         //Free the memory space pointed by the given pointer
};
#endif