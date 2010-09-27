#include "NMS_Audio.h"


void NMS_Audio::LoadWav(char* sFileName,char* sSoundName,ALfloat* pSourcePos,ALfloat* pSourceVel,float fPitch,float fGain,bool loop)
{

	//Check if the name is already inside our map, if it is, generate an error
	std::map<char* ,sourceStruct>::iterator iter = sourceMap.find(sSoundName);
	if( iter != sourceMap.end() ) 
	{
		throw 0;
	}


    // Buffer id and error checking variable.
	sourceStruct source;
    ALenum result;

	//Define the position of the source of the sound
	source.fSourcePos=pSourcePos;
	source.fSourceVel=pSourceVel;
	source.sSourceName=sSoundName;

    // Generate a buffer. Check that it was created successfully.
	source.iBufferID=NMS_SOUNDMANAGER.LoadWav(sFileName,sSoundName);

    if ((result = alGetError()) != AL_NO_ERROR)
        throw 0;//GetALErrorString(result);

    // Generate a source.
	alGenSources(1,&source.iSourceID);

    if ((result = alGetError()) != AL_NO_ERROR)
        throw 0;// GetALErrorString(result);

    // Setup the source properties.
	alSourcei (source.iSourceID, AL_BUFFER,   source.iBufferID   );
	alSourcef (source.iSourceID, AL_PITCH,    fPitch   );
    alSourcef (source.iSourceID, AL_GAIN,     fGain    );
    alSourcefv(source.iSourceID, AL_POSITION, source.fSourcePos);
    alSourcefv(source.iSourceID, AL_VELOCITY, source.fSourceVel);
    alSourcei (source.iSourceID, AL_LOOPING,  loop     );

	sourceMap[sSoundName]=source;
}


void NMS_Audio::SetListenerValues(ALfloat* pListenerPos,ALfloat* pListenerVel,ALfloat* pListenerOr)
{
    alListenerfv(AL_POSITION,    pListenerPos);
    alListenerfv(AL_VELOCITY,    pListenerVel);
    alListenerfv(AL_ORIENTATION, pListenerOr);
}

void NMS_Audio::playSound(char* sSoundName)
{
	//Check if the name is already inside our map, if it not, generate an error
	std::map<char* ,sourceStruct>::iterator iter = sourceMap.find(sSoundName);
	if( iter == sourceMap.end() ) 
	{
		throw 0;
	}
	alSourcePlay(iter->second.iSourceID);
}

void NMS_Audio::pauseSound(char* sSoundName)
{
	//Check if the name is already inside our map, if it not, generate an error
	std::map<char* ,sourceStruct>::iterator iter = sourceMap.find(sSoundName);
	if( iter == sourceMap.end() ) 
	{
		throw 0;
	}
	alSourcePause(iter->second.iSourceID);
}

void NMS_Audio::stopSound(char* sSoundName)
{
	//Check if the name is already inside our map, if it not, generate an error
	std::map<char* ,sourceStruct>::iterator iter = sourceMap.find(sSoundName);
	if( iter == sourceMap.end() ) 
	{
		throw 0;
	}
	alSourceStop(iter->second.iSourceID);
}

NMS_Audio::NMS_Audio()
{
	//Just initialize the listener, just in case the user do not do that
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

	// Velocity of the listener.
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

	// Orientation of the listener. (first 3 elements are "at", second 3 are "up")
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

	NMS_Audio(ListenerPos,ListenerVel,ListenerOri);
}

NMS_Audio::NMS_Audio(ALfloat* pListenerPos,ALfloat* pListenerVel,ALfloat* pListenerOri)
{
	// Initialize OpenAL and clear the error bit.
	alutInit(0, NULL);
	alGetError();
	SetListenerValues(pListenerPos,pListenerVel,pListenerOri);
}