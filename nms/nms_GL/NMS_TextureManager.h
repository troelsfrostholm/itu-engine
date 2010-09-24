#pragma once

#include "NMS_sha1.h"
#include "NMS_FileManagement.h"


#include <string.h>
#include <stdio.h>

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>


//Include Devil Libraries
#include <IL/il.h>
		
#define NMS_TEXTUREMANAGER	NMS_TextureManager::GetSingleton()
#define DESTROY_NMS_TEXTUREMANAGER	NMS_TextureManager::Destroy();

//#define UBYTE unsigned char
#define INITIAL_SIZE	32	// initial size of the TexID array
#define TEXTURE_STEP	8	// how much the array grows by each time

class __declspec(dllexport) NMS_TextureManager {
public :
	NMS_TextureManager (void);
	~NMS_TextureManager (void);
	static NMS_TextureManager &GetSingleton (void);

private :
	static void Initialize (void);
public :
	static void Destroy (void);

public :
	//Load a texture. We are using devil so we can load any file format that Devil is able to load as a texture
	int LoadTexture (const char *szFilename, int nTextureID = -1);
	//Release the memory space occupied by a specific texture
	void FreeTexture (int nID);
	//Release the memory occupied by all the textures
	void FreeAll (void);

public :	// Debug / Utilitarian
	char *GetErrorMessage (void);
	
	int	GetNumTextures (void);
	int GetAvailableSpace (void);
	int GetTexID (int nIndex);

private :
	static NMS_TextureManager *m_Singleton;

	//Get a texture ID and create one if it is not provide
	int GetNewTextureID (int nPossibleTextureID);	// get a new one if one isn't provided
	bool CheckSize (int nDimension);

private :
	char szErrorMessage [80];
	static const size_t szErrorMessageSize = 80*sizeof(char);
	int nNumTextures;
	int nAvailable;				// available space in the nTexID array
	int *nTexIDs;
};