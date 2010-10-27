#ifdef __EXP_NMS_GL
#    define ASSETMANAGER_D __declspec(dllexport)
#else
#    define ASSETMANAGER_D __declspec(dllimport)
#endif

#ifndef __NMS_ASSETMANAGER
#define __NMS_ASSETMANAGER

#include <string.h>
#include <stdio.h>
#include <map>

#define NOMINMAX

#include <Windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "NMS_TextureManager.h"


#define NMS_ASSETMANAGER	    NMS_AssetManager::GetSingleton()
#define DESTROY_ASSETMANAGER	NMS_AssetManager::Destroy()


class ASSETMANAGER_D NMS_AssetManager{
public :
	NMS_AssetManager (void);
	~NMS_AssetManager (void);
	static NMS_AssetManager& GetSingleton (void);


private :	
	//Initialization function, called automatically when the Singletone Assetmanager is called
	static void Initialize (void);
public :
	static void Destroy (void);

public :	
	int  LoadTexture   (const char *p_Filename,  char* textureName);
	int  LoadSound     (const char* p_fileName,int iSoundID=-1);
	int  LoadMD2Model  (const char* p_fileName,int iModelID=-1);
	void FreeTexture   (char* textureName);
	void FreeSound     (int nID);
	void FreeMD2Model  (int nID);
	void FreeAll       (void);

public :
	char *GetErrorMessage (void);
	
private :
	static NMS_AssetManager *m_Singleton;

private :
	char szErrorMessage [80];
	static const size_t szErrorMessageSize = 80*sizeof(char);
};
#endif
