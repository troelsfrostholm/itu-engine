#include "NMS_FileManagement.h"


long nmsFileManagement::FileSize(FILE *fp)
{
	//Return the filesize of the file by seeking to the end of the file
	long oldpos=ftell(fp);
	fseek(fp,0,SEEK_END);
	long curpos=ftell(fp);
	fseek(fp,oldpos,SEEK_SET);
	return curpos;
}

bool nmsFileManagement::FileExists(const char * filename)
{
    if (FILE * file = fopen(filename, "r"))
    {
        fclose(file);
        return true;
    }
    return false;
}

//Allocate the memory space with regard to the type to be used. Return a pointer to the memory allocation
void* nmsFileManagement::Malloc(size_t size)
{
	void* p=malloc(size);
	if (p==NULL)
		return p;
	memset(p,0,size);
	return p;
}

//Free the memory space pointed by the given pointer. Set to void** to receive every kind of pointers
void nmsFileManagement::Free(void** p)
{
	if (*p!=NULL)
	{
		free(*p);
		*p=NULL;
	}
}