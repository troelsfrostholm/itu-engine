#include "NMS_TextureManager.h"

NMS_TextureManager *NMS_TextureManager::m_Singleton = 0;

NMS_TextureManager::NMS_TextureManager (void) {}

NMS_TextureManager::~NMS_TextureManager (void) {}

NMS_TextureManager& NMS_TextureManager::GetSingleton (void) {
	if (!m_Singleton) {
		m_Singleton = new(STATIC_ALLOC, MEM_PERSISTENT) NMS_TextureManager;
		Initialize ();
	}
	return *m_Singleton;
}

void NMS_TextureManager::Initialize (void) {
	m_Singleton->m_sMessage[0]='\0';
	LOG.write("NMS_TextureManager::Texture Manager Initialized!\n",LOG_DEBUG);
}

void NMS_TextureManager::Destroy (void) {
	if (m_Singleton) {
		m_Singleton->FreeAll();
		delete m_Singleton;
		m_Singleton = 0;
		LOG.write("NMS_TextureManager::Texture Manager Destroyed!\n",LOG_DEBUG);
	}
}

int NMS_TextureManager::LoadTexture (const char* sFilename,char* textureName) {
	if(_DEBUG)
	{
		sprintf_s (m_Singleton->m_sMessage, m_Singleton->m_iMessageSize, "NMS_TextureManager::Trying to load [%s]\n", sFilename);
		LOG.write(m_sMessage,LOG_DEBUG);
	}
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	  {
		  LOG.write("NMS_TextureManager::Wrong DevIL version!\n",LOG_ERROR);
		  return -1;
	  }
	 
	  //Create the hash on which to compare to see if we have loaded the same two file twice
	  shaMap hash;

	  //Open the file provided and check for the SHA1 hash to see if we have already another texture like this
	  FILE	*fp=NULL;
	  ILubyte *Lump;

	  //Open the texture file
	  fopen_s(&fp,sFilename,"rb");
	  if (!fp)
	  {
		LOG.write("NMS_TextureManager::Cannot open the texture file!\n",LOG_ERROR);
		return -1;
	  }
	 

	  //Calculate the size of the file
	  long fileSize= nmsFileManagement::FileSize(fp);
	  if (fileSize<=0)
	  {
		  LOG.write("NMS_TextureManager::Negative file size for the texture!\n",LOG_ERROR);
		  return -1;
	  }

	 

	  //Convert the file read to a Lump file to be used by DevIL
	  Lump = (ILubyte*)LEVEL_ALLOC->allocMem(fileSize);
	  fseek(fp, 0, SEEK_SET);
	  fread(Lump, 1, fileSize, fp);
	  fclose(fp);
	  if(_DEBUG)
			LOG.write("NMS_TextureManager::Lump created correctly!\n",LOG_DEBUG);



	  //Create the sha1
	  hash=nmsSha1::returnSha1(Lump,fileSize);
	  textStruct imageToBeAdded=checkForHash(hash,textureName);

	  GLuint image=NULL;
	  ILuint texid=NULL;
	  //Just a basic check. If both the textID AND the hash are different from NULL that means that 
	  //we have already the texture and we just want to change the name into the map
	  if(imageToBeAdded.textID!=NULL && imageToBeAdded.hash!=NULL)
	  {
		  textureMap[textureName]=imageToBeAdded;
		  image=textureMap[textureName].textID;
	  }
	  else
	  {
		  //It's the same texture with the same name, do nothing!
		  if(imageToBeAdded.textID==NULL && imageToBeAdded.hash!=NULL)
		  {
			  image=textureMap[textureName].textID;
		  }
		  //It's a different texture with the same name, warn the user and exit
		  else if(imageToBeAdded.textID==-1)
			    {throw 0;}
		  else
			  {
				  //It's a new image, load it
				  ILboolean success;
				  ilInit(); /* Initialization of DevIL */
				  ilGenImages(1, &texid); /* Generation of one image name */
				  ilBindImage(texid); /* Binding of image name */
				  success = ilLoadL(IL_TYPE_UNKNOWN,Lump,fileSize); /* Loading of image "image.jpg" */  //USA iLoadImageF
				  //free(Lump);
				  

				  if (success) /* If no error occured: */
				  {
					success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
					  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
					if (!success)
					{
						LOG.write("NMS_TextureManager::Error in converting the texture in the proper format!\n",LOG_ERROR);
						ilDeleteImages(1, &texid);					 
						/* Error occured */
						return -1;
					}


					//Create the new image
					glGenTextures(1, &image); /* Texture name generation */
					glBindTexture(GL_TEXTURE_2D, image);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
					  /* We will use linear interpolation for magnification filter */
					  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
					  /* We will use linear interpolation for minifying filter */
					  glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
							 ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
							 ilGetData()); /* Texture specification */



					if(_DEBUG)
						LOG.write("NMS_TextureManager::Texture loaded from Lump!\n",LOG_DEBUG);
				  }
				  else
				  {
					/* Error occured */
					LOG.write("NMS_TextureManager::Error in loading the texture from the Lump!\n",LOG_ERROR);
					ilDeleteImages(1, &texid);
					return -1;
				  }
			  }
	  }
	  //Pick the image that has been created to be used in our context
	  glBindTexture(GL_TEXTURE_2D, image); /* Binding of texture name */
	  textureMap[textureName].textID=image;
	  textureMap[textureName].hash=hash;
	  ilDeleteImages(1, &texid);
	  //Close the file we are done with it
	  fclose(fp);
	  if(_DEBUG)
	  {
		  sprintf_s (m_Singleton->m_sMessage, m_Singleton->m_iMessageSize, "NMS_TextureManager::Loaded [%s] without issues!\n", sFilename);
		  LOG.write(m_sMessage,LOG_DEBUG);
	  }
	  return textureMap[textureName].textID;
}

void NMS_TextureManager::FreeTexture (char* textureName) {
	    std::map<char* ,textStruct>::iterator iter = textureMap.find(textureName);
		if( iter != textureMap.end() ) {
			//Delete the requested texture
			glDeleteTextures (1, &textureMap[textureName].textID);
			if(_DEBUG)
			{
				 sprintf_s (m_Singleton->m_sMessage, m_Singleton->m_iMessageSize, "NMS_TextureManager::Deleted image [%s] without issues!\n", textureName);
				 LOG.write(m_sMessage,LOG_DEBUG);
			}
		}
		  
}

void NMS_TextureManager::FreeAll (void) {
	 std::map<char* ,textStruct>::iterator iter;
     for( iter = textureMap.begin(); iter != textureMap.end(); ++iter ) 
	 {
      glDeleteTextures (1, &textureMap[iter->first].textID);
     }
	 if(_DEBUG)
	 {
		LOG.write("NMS_TextureManager::Freed the whole memory!\n",LOG_DEBUG);
	 }
}


textStruct NMS_TextureManager::checkForHash(shaMap hash,char* textureName)
{
	shaMap current=NULL;
	textStruct toBeReturned;
	toBeReturned.hash=NULL;
	toBeReturned.textID=NULL;
	std::map<char*, textStruct>::const_iterator itr;
	for(itr = textureMap.begin(); itr != textureMap.end(); ++itr)
	{
		if(0==strcmp(hash,itr->second.hash))
		{
			if(0==strcmp(itr->first,textureName))
				toBeReturned.textID=NULL;
			else
				toBeReturned.textID=itr->second.textID;
			//We have found a corresponding texture, just return the ID of the image that is in memory
			toBeReturned.hash=itr->second.hash;
			return toBeReturned;
		}
		else 
			//It's an error the user cannot use the same name for two different files!
			//Warn the user
			if(0==strcmp(itr->first,textureName))
			{
				LOG.write("NMS_TextureManager::Impossible to assign the same name to two different textures!\n",LOG_RUN);
				toBeReturned.textID=-1;
				return toBeReturned;
			}
	}
	return toBeReturned;
}
