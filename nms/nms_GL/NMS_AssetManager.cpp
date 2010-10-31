#include "NMS_AssetManager.h"

NMS_AssetManager *NMS_AssetManager::m_Singleton = 0;


//NOTE: The constructor is NEVER called directly because this is a singletone class.
//When the GetSingletone command is issue the constructor will be called, to initialize data just use Initialize
NMS_AssetManager::NMS_AssetManager (void) {
	szErrorMessage [0] = '\0';
}

NMS_AssetManager::~NMS_AssetManager (void) {

}

NMS_AssetManager& NMS_AssetManager::GetSingleton (void) {
	if (!m_Singleton) {
		m_Singleton = new(STATIC_ALLOC, MEM_SINGLETON) NMS_AssetManager;
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


char* NMS_AssetManager::GetErrorMessage (void) {
	return m_Singleton->szErrorMessage;
}

int  NMS_AssetManager::LoadTexture(const char *p_Filename, char* textureName)
{
	return NMS_TEXTUREMANAGER.LoadTexture(p_Filename,textureName);
}

void NMS_AssetManager::FreeTexture   (char* textureName)
{
	NMS_TEXTUREMANAGER.FreeTexture(textureName);
}

void NMS_AssetManager::FreeAll()
{
	NMS_TEXTUREMANAGER.FreeAll();
	//Remember to release also the other assets!
}