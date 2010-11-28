#ifdef __EXP_NMS_GL
#    define TEXTUREMANAGER_D __declspec(dllexport)
#else
#    define TEXTUREMANAGER_D __declspec(dllimport)
#endif

#ifndef __NMS_TEXTUREMANAGER
#define __NMS_TEXTUREMANAGER

#pragma warning( disable: 4251 )  //Used to disable this useless warning: http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html

#include "NMS_sha1.h"
#include "NMS_FileManagement.h"
#include "NMS_LogFunctions.h"
#include "NMS_CustomTypes.h"
#include "NMS_StaticAllocator.h"
#include "NMS_LevelAllocator.h"

#include <map>
#include <string.h>
#include <stdio.h>

#include "SDL_opengl.h"



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

class TEXTUREMANAGER_D NMS_TextureManager {
public :

	static NMS_TextureManager& GetSingleton (void);

private :
	NMS_TextureManager (void);
	~NMS_TextureManager (void);
	static void Initialize (void);
	textStruct checkForHash(shaMap hash,string textureName);

public :
	//Load a texture. We are using devil so we can load any file format that Devil is able to load as a texture
	int LoadTexture (const char *szFilename,char* textureName);
	int LoadTextureNoBind (const char *szFilename,char* textureName);
	//Release the memory space occupied by a specific texture
	void FreeTexture (char* textureName);
	//Release the memory occupied by all the textures
	void FreeAll (void);
	static void Destroy (void);

private :
	static NMS_TextureManager* m_Singleton;
	char m_sMessage[80];
	static const size_t m_iMessageSize=80*sizeof(char);

	struct stringLess : binary_function <string, string, bool> {
		bool operator() (const string& x, const string& y) const
			{ return x.compare(y)<0; }
	};
	std::map<string, textStruct, stringLess> textureMap;
};
#endif