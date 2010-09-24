#include <string.h>
#include <stdio.h>

#include "NMS_TextureManager.h"


#define NMS_ASSETMANAGER	    NMS_AssetManager::GetSingleton();
#define DESTROY_ASSETMANAGER	NMS_AssetManager::Destroy();


class __declspec(dllexport) NMS_AssetManager {
public :
	NMS_AssetManager (void);
	~NMS_AssetManager (void);
	static NMS_AssetManager &GetSingleton (void);

private :	
	//Initialization function, called automatically when the Singletone Assetmanager is called
	static void Initialize (void);
public :
	static void Destroy (void);

public :	
	int  NMS_AssetManager::LoadTexture   (const char *p_Filename, int iTextureID = -1);
	int  NMS_AssetManager::LoadSound     (const char* p_fileName,int iSoundID=-1);
	int  NMS_AssetManager::LoadMD2Model  (const char* p_fileName,int iModelID=-1);
	void NMS_AssetManager::FreeTexture   (int nID);
	void NMS_AssetManager::FreeSound     (int nID);
	void NMS_AssetManager::FreeMD2Model  (int nID);
	void NMS_AssetManager::FreeAll       (void);

public :	// Debug / Utilitarian
	char *GetErrorMessage (void);
	
private :
	static NMS_AssetManager *m_Singleton;

private :
	char szErrorMessage [80];
	static const size_t szErrorMessageSize = 80*sizeof(char);
};
