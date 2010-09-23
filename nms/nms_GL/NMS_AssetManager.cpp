#include "NMS_AssetManager.h"

/**
	Note : Being a singleton all the data we 'want' is located
	in NMS_AssetManager::m_Singleton, so although it looks really
	ugly to have so many 'm_Singleton->'s this is so the code will
	actually work as designed ;)
**/

NMS_AssetManager *NMS_AssetManager::m_Singleton = 0;
// ===================================================================
/**
No use for a constructor because this singlton is never created,
when GetSingleton is called for the first time and issued a 'new'
command the constructor is called, however because of the memory
isn't valid until AFTER the constructor it just is a bad idea...
Use Initialize and Destroy for your dirty work
**/
NMS_AssetManager::NMS_AssetManager (void) {
	// This is just to be clean, but all 'real' data
	// is in m_Singleton
	
	szErrorMessage [0] = '\0';
}

NMS_AssetManager::~NMS_AssetManager (void) {

}

NMS_AssetManager &NMS_AssetManager::GetSingleton (void) {
	if (!m_Singleton) {
		m_Singleton = new NMS_AssetManager;
		Initialize ();
	}

	return *m_Singleton;
}

void NMS_AssetManager::Initialize (void) {
	sprintf_s (m_Singleton->szErrorMessage, NMS_AssetManager::szErrorMessageSize, "Asset Manager Initialized!");
}

void NMS_AssetManager::Destroy (void) {
	if (m_Singleton) {
		delete m_Singleton;
		m_Singleton = 0;
	}
}

// ===================================================================


// ===================================================================

char *NMS_AssetManager::GetErrorMessage (void) {
	return m_Singleton->szErrorMessage;
}

//int  NMS_AssetManager::LoadTexture(const char *p_Filename, int iTextureID = -1)
//{
//	return NMS_TEXTUREMANAGER.LoadTexture(p_Filename,iTextureID);
//}

void NMS_AssetManager::FreeTexture   (int nID)
{
	NMS_TEXTUREMANAGER.FreeTexture(nID);
}

void NMS_AssetManager::FreeAll()
{
	NMS_TEXTUREMANAGER.FreeAll();
}