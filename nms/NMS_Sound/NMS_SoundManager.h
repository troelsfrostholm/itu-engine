#ifndef __NMS_SOUNDMANAGER
#define __NMS_SOUNDMANAGER

#include "NMS_sha1.h"
#include "NMS_FileManagement.h"
#include "NMS_LogFunctions.h"
#include "NMS_CustomTypes.h"

#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>




#include <vector>
#include <map>
#include <string.h>
#include <stdio.h>

// Maximum data buffers we will need.
#define MAX_BUFFERS 50

// Maximum emissions we will need.
#define MAX_SOURCES 50

typedef ALfloat ALPos[3];

typedef struct
{
	ALuint  sourceID;
	//Position of the source
	ALenum  SourcesPos;
	//Velocity of the source
    ALsizei SourcesVel;
}sourceStruct;


//This structure contains the ID of the sound together with the hash and other important informations
typedef struct
{
	ALuint  soundID;
	shaMap hash;
}soundStruct;

		
#define NMS_SOUNDMANAGER	NMS_SoundManager::GetSingleton()
#define DESTROY_NMS_SOUNDMANAGER	NMS_SoundManager::Destroy()



class __declspec(dllexport) NMS_SoundManager {
public :
	NMS_SoundManager (void);
	~NMS_SoundManager (void);
	static NMS_SoundManager &GetSingleton (void);

private :
	static void Initialize (void);
	soundStruct checkForHash(shaMap hash,char* textureName);
public :
	static void Destroy (void);

public :
	//Load a sound. We are using openAL so we have to specify the formats that our engine is able to load or not
	//Up to now only wavs files are supported
	ALuint LoadWav(const char* sFileName,char* sSoundName);
	//Release the memory space occupied by a specific texture
	void FreeSound (char* soundName);
	//Release the memory occupied by all the textures
	void FreeAll (void);

private :
	static NMS_SoundManager *m_Singleton;


	// Buffers hold sound data.
	std::vector<ALuint> Buffers;
	// Sources are points of emitting sound.
	std::vector<sourceStruct> Sources;


	char m_sMessage[80];
	static const size_t m_iMessageSize=80*sizeof(char);
	std::map<char* ,soundStruct> soundMap;
};
#endif