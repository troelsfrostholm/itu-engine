#include "NMS_AssetManager.h"

#define ASSETMANAGER	NMS_AssetManager::GetSingleton();
#define DESTROY_ASSETMANAGER	NMS_AssetManager::Destroy();


class NMS_AssetManager {
public :
	NMS_AssetManager (void);
	~NMS_AssetManager (void);
	static NMS_AssetManager &GetSingleton (void);

private :	// This is called automaticaly! Don't do it yourself!
	static void Initialize (void);
public :
	static void Destroy (void);

public :	// Usage / Implumentation
	int LoadTexture (const char *szFilename, int nTextureID = -1);

	void FreeTexture (int nID);
	void FreeAll (void);

public :	// Debug / Utilitarian
	char *GetErrorMessage (void);
	
	int	GetNumTextures (void);
	int GetAvailableSpace (void);
	int GetTexID (int nIndex);

private :
	static NMS_TextureManager *m_Singleton;

	int GetNewTextureID (int nPossibleTextureID);	// get a new one if one isn't provided
	bool CheckSize (int nDimension);

private :
	char szErrorMessage [80];
	int nNumTextures;
	int nAvailable;				// available space in the nTexID array
	int *nTexIDs;
};
