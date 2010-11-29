#include "NMS_SoundManager.h"

NMS_SoundManager* NMS_SoundManager::m_Singleton = 0;

NMS_SoundManager::NMS_SoundManager (void) {}

NMS_SoundManager::~NMS_SoundManager (void) {}

NMS_SoundManager& NMS_SoundManager::GetSingleton (void) {
	if (!m_Singleton) {
		m_Singleton = new(STATIC_ALLOC, MEM_PERSISTENT) NMS_SoundManager;
		Initialize ();
	}
	return *m_Singleton;
}

void NMS_SoundManager::Initialize (void) {
	m_Singleton->m_sMessage[0]='\0';
	LOG.write("NMS_SoundManager::Sound Manager Initialized!\n",LOG_DEBUG);
}

void NMS_SoundManager::Destroy (void) {
	if (m_Singleton) {
		m_Singleton->FreeAll();
		delete m_Singleton;
		m_Singleton = 0;
		LOG.write("NMS_SoundManager::Sound Manager Destroyed!\n",LOG_DEBUG);
	}
}


ALuint NMS_SoundManager::LoadWav (const char* sFileName,char* sSoundName) 
{
	if(_DEBUG)
	{
		sprintf_s (m_Singleton->m_sMessage, m_Singleton->m_iMessageSize, "NMS_SoundManager::Trying to load [%s]\n", sFileName);
		LOG.write(m_sMessage,LOG_DEBUG);
	}

	  //Create the hash on which to compare to see if we have loaded the same two file twice
	  shaMap hash;

	  //Open the file provided and check for the SHA1 hash to see if we have already another texture like this
	  FILE	*fp=NULL;
	  ALbyte* soundMemory;

	  //Open the sound file
	  fopen_s(&fp,sFileName,"rb");
	  if (!fp)
	  {
		LOG.write("NMS_SoundManager::Cannot open the sound file!\n",LOG_ERROR);
		return -1;
	  }

	  //Calculate the size of the file
	  long fileSize= nmsFileManagement::FileSize(fp);
	  if (fileSize<=0)
	  {
		  LOG.write("NMS_SoundManager::Negative file size for the sound!\n",LOG_ERROR);
		  return -1;
	  }

	   //Convert the file read to a Lump file to be used by DevIL
	  soundMemory = (ALbyte*)LEVEL_ALLOC->allocMem(fileSize, MEM_LEVEL);
	  fseek(fp, 0, SEEK_SET);
	  fread(soundMemory, 1, fileSize, fp);
	  fclose(fp);
	  if(_DEBUG)
			LOG.write("NMS_TextureManager::Lump created correctly!\n",LOG_DEBUG);

	  //Create the sha1
	  hash=nmsSha1::returnSha1(soundMemory,fileSize);
	  soundStruct soundToBeAdded=checkForHash(hash,sSoundName);

	  ALuint buffer;

	  //Just a basic check. If both the textID AND the hash are different from NULL that means that 
	  //we have already the texture and we just want to change the name into the map
	  if(soundToBeAdded.soundID!=NULL && soundToBeAdded.hash!=NULL)
	  {
		  soundMap[sSoundName]=soundToBeAdded;
		  buffer=soundMap[sSoundName].soundID;
	  }
	  else
	  {
		  //It's the same texture with the same name, do nothing!
		  if(soundToBeAdded.soundID==NULL && soundToBeAdded.hash!=NULL)
		  {
			  return soundMap[sSoundName].soundID;
		  }
		  //It's a different texture with the same name, warn the user and exit
		  else if(soundToBeAdded.soundID==-1)
			    {throw 0;}
		  else
			  {
				  //It's a new sound, load it

				  ALenum result;
				  ALenum format;
					ALsizei size;
					ALvoid* data;
					ALsizei freq;
					ALboolean loop;


					// Generate a buffer. Check that it was created successfully.
				  alGenBuffers(1, &buffer);

				  if ((result = alGetError()) != AL_NO_ERROR)
						throw 0;//GetALErrorString(result);

					// Read in the wav data from file. Check that it loaded correctly.
				   //NOTE: CANNOT LOAD FROM MEMORY, THERE'S A BUG WITH OPENAL loadWAVMemory, we are forced to load the 
				  //file twice!
				   alutLoadWAVFile((ALbyte*)sFileName, &format, &data, &size, &freq, &loop);
				   //Free the allocated space in memory
				   //free(soundMemory);

				   if ((result = alGetError()) != AL_NO_ERROR)
						throw 0;//GetALErrorString(result);

					// Send the wav data into the buffer. Check that it was received properly.
					alBufferData(buffer, format, data, size, freq);

					if ((result = alGetError()) != AL_NO_ERROR)
						throw 0;//GetALErrorString(result);

					// Get rid of the temporary data.
					alutUnloadWAV(format, data, size, freq);

					if ((result = alGetError()) != AL_NO_ERROR)
						throw 0;//GetALErrorString(result);
		  }
	  }
	  soundMap[sSoundName].soundID=buffer;
	  soundMap[sSoundName].hash=hash;
	  //Close the file we are done with it
	  fclose(fp);
	  if(_DEBUG)
	  {
		  sprintf_s (m_Singleton->m_sMessage, m_Singleton->m_iMessageSize, "NMS_SoundManager::Loaded [%s] without issues!\n", sFileName);
		  LOG.write(m_sMessage,LOG_DEBUG);
	  }
	  return soundMap[sSoundName].soundID;
}

void NMS_SoundManager::FreeSound (char* sSoundName) {
	    std::map<char* ,soundStruct>::iterator iter = soundMap.find(sSoundName);
		if( iter != soundMap.end() ) {
			//Delete the requested texture
			alDeleteBuffers(1, &soundMap[iter->first].soundID);
			if(_DEBUG)
			{
				 sprintf_s (m_Singleton->m_sMessage, m_Singleton->m_iMessageSize, "NMS_SoundManager::Deleted sound [%s] without issues!\n", sSoundName);
				 LOG.write(m_sMessage,LOG_DEBUG);
			}
		}
		  
}

void NMS_SoundManager::FreeAll (void) {
	 std::map<char* ,soundStruct>::iterator iter;
     for( iter = soundMap.begin(); iter != soundMap.end(); ++iter ) 
	 {
        alDeleteBuffers(1, &soundMap[iter->first].soundID);
     }
	 if(_DEBUG)
	 {
		LOG.write("NMS_SoundManager::Freed the whole memory!\n",LOG_DEBUG);
	 }
}


soundStruct NMS_SoundManager::checkForHash(shaMap hash,char* sSoundName)
{
	shaMap current=NULL;
	soundStruct toBeReturned;
	toBeReturned.hash=NULL;
	toBeReturned.soundID=NULL;
	std::map<char*, soundStruct>::const_iterator itr;
	for(itr = soundMap.begin(); itr != soundMap.end(); ++itr)
	{
		if(0==strcmp(hash,itr->second.hash))
		{
			if(0==strcmp(itr->first,sSoundName))
				toBeReturned.soundID=NULL;
			else
				toBeReturned.soundID=itr->second.soundID;
			//We have found a corresponding texture, just return the ID of the image that is in memory
			toBeReturned.hash=itr->second.hash;
			return toBeReturned;
		}
		else 
			//It's an error the user cannot use the same name for two different files!
			//Warn the user
			if(0==strcmp(itr->first,sSoundName))
			{
				LOG.write("NMS_SoundManager::Impossible to assign the same name to two different sounds!\n",LOG_RUN);
				toBeReturned.soundID=-1;
				return toBeReturned;
			}
	}
	return toBeReturned;
}
