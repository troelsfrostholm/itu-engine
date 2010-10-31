/****************************************************************************************
DLL EXPORTING SAFE
*****************************************************************************************/

#ifdef __EXP_NMS_GL
#    define ASSETMANAGER_D __declspec(dllexport)
#else
#    define ASSETMANAGER_D __declspec(dllimport)
#endif

#ifndef __NMS_ASSETMANAGER
#define __NMS_ASSETMANAGER


#define NOMINMAX

#include "NMS_TextureManager.h"
#include "NMS_StaticAllocator.h"


#define NMS_ASSETMANAGER	    NMS_AssetManager::GetSingleton()
#define DESTROY_ASSETMANAGER	NMS_AssetManager::Destroy()


class ASSETMANAGER_D NMS_AssetManager{
public :
	static NMS_AssetManager& GetSingleton (void);
	int  LoadTexture   (const char* p_Filename,char* textureName);
	int  LoadSound     (const char* p_fileName,int iSoundID=-1);
	int  LoadMD2Model  (const char* p_fileName,int iModelID=-1);
	void FreeTexture   (char* textureName);
	void FreeSound     (int nID);
	void FreeMD2Model  (int nID);
	void FreeAll       (void);
	static void Destroy (void);
	char* GetErrorMessage (void);
	
private :
	static NMS_AssetManager* m_Singleton;
	char szErrorMessage [80];
	static const size_t szErrorMessageSize = 80*sizeof(char);

	//Initialization function, called automatically when the Singletone Assetmanager is called
	static void Initialize (void);
	NMS_AssetManager (void);
	~NMS_AssetManager (void);
};
#endif
