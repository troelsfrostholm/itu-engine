#include <string.h>
#include <stdio.h>

#include <iostream>	// DEBUG ONLY

#include "NMS_TextureManager.h"


#define NMS_ASSETMANAGER	NMS_AssetManager::GetSingleton();
#define DESTROY_ASSETMANAGER	NMS_AssetManager::Destroy();


class __declspec(dllexport) NMS_AssetManager {
public :
	NMS_AssetManager (void);
	~NMS_AssetManager (void);
	static NMS_AssetManager &GetSingleton (void);

private :	// This is called automaticaly! Don't do it yourself!
	static void Initialize (void);
public :
	static void Destroy (void);

public :	// Usage / Implementation
	int  NMS_AssetManager::LoadTexture   (const char *p_Filename, int iTextureID = -1);
	int  LoadSound     (const char* p_fileName,int iSoundID=-1);
	int  LoadMD2Model  (const char* p_fileName,int iModelID=-1);
	void FreeTexture   (int nID);
	void FreeSound     (int nID);
	void FreeMD2Model  (int nID);
	void FreeAll       (void);

public :	// Debug / Utilitarian
	char *GetErrorMessage (void);
	
private :
	static NMS_AssetManager *m_Singleton;

private :
	char szErrorMessage [80];
};
