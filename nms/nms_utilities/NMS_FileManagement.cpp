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

