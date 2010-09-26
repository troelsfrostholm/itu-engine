#ifndef __NMS_TEXTUREMANAGER
#define __NMS_TEXTUREMANAGER

#include "NMS_sha1.h"
#include "NMS_FileManagement.h"
#include "NMS_LogFunctions.h"
#include "NMS_CustomTypes.h"

#include <map>
#include <string.h>
#include <stdio.h>

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>


//Include Devil Libraries
#include <IL/il.h>
		
#define NMS_TEXTUREMANAGER	NMS_TextureManager::GetSingleton()
#define DESTROY_NMS_TEXTUREMANAGER	NMS_TextureManager::Destroy()


//This structure contains the ID of the texture together with the hash of the corresponding texture
typedef struct
{
	GLuint textID;
	shaMap hash;
}textStruct;

class __declspec(dllexport) NMS_TextureManager {
public :
	NMS_TextureManager (void);
	~NMS_TextureManager (void);
	static NMS_TextureManager &GetSingleton (void);

private :
	static void Initialize (void);
	textStruct checkForHash(shaMap hash,char* textureName);
public :
	static void Destroy (void);

public :
	//Load a texture. We are using devil so we can load any file format that Devil is able to load as a texture
	int LoadTexture (const char *szFilename,char* textureName);
	//Release the memory space occupied by a specific texture
	void FreeTexture (char* textureName);
	//Release the memory occupied by all the textures
	void FreeAll (void);

private :
	static NMS_TextureManager *m_Singleton;

	//Get a texture ID
	int GetNewTextureID ();
	bool CheckSize (int nDimension);


	char m_sMessage[80];
	static const size_t m_iMessageSize=80*sizeof(char);
	std::map<char* ,textStruct> textureMap;
};
#endif