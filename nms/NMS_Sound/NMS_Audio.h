#ifdef __EXP_NMS_SOUND
#    define AUDIO_D __declspec(dllexport)
#else
#    define AUDIO_D __declspec(dllimport)
#endif

#ifndef __NMS_AUDIO
#define __NMS_AUDIO

#include "NMS_SoundManager.h"
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>
#include <vector>



typedef struct
{
	ALuint  iSourceID;
	ALuint  iBufferID;
	char*   sSourceName;
	//Position of the source
	ALfloat* fSourcePos;
	//Velocity of the source
    ALfloat* fSourceVel;
}sourceStruct;

typedef struct
{
	// Position of the listener.
	ALfloat ListenerPos[3];

	// Velocity of the listener.
	ALfloat ListenerVel[3];

	// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
	ALfloat ListenerOri[6];

}listenerStruct;

class AUDIO_D NMS_Audio
{
	public:
		NMS_Audio();
		NMS_Audio::NMS_Audio(ALfloat* pListenerPos,ALfloat* pListenerVel,ALfloat* pListenerOri);
		void LoadWav(char* sFileName,char* sSoundName,ALfloat* pSourcePos,ALfloat* pSourceVel,float fPitch,float fGain,bool loop);
		void SetListenerValues(ALfloat* pListenerPos,ALfloat* pListenerVel,ALfloat* pListenerOr);
		void playSound(char* sSoundName);
		void pauseSound(char* sSoundName);
		void stopSound(char* sSoundName);
	private:
		std::map<char* ,sourceStruct> sourceMap;
		//Properties related to the listener
		listenerStruct listenerProperties;
};
#endif