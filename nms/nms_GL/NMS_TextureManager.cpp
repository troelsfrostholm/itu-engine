#include "NMS_TextureManager.h"


/**
	Note : Being a singleton all the data we 'want' is located
	in NMS_TextureManager::m_Singleton, so although it looks really
	ugly to have so many 'm_Singleton->'s this is so the code will
	actually work as designed ;)
**/

NMS_TextureManager *NMS_TextureManager::m_Singleton = 0;
// ===================================================================
/**
No use for a constructor because this singlton is never created,
when GetSingleton is called for the first time and issued a 'new'
command the constructor is called, however because of the memory
isn't valid until AFTER the constructor it just is a bad idea...
Use Initialize and Destroy for your dirty work
**/
NMS_TextureManager::NMS_TextureManager (void) {
	// This is just to be clean, but all 'real' data
	// is in m_Singleton
	
	szErrorMessage [0] = '\0';
	nNumTextures	   = 0;
	nAvailable     	   = 0;
	nTexIDs            = 0;
}

NMS_TextureManager::~NMS_TextureManager (void) {

}

NMS_TextureManager &NMS_TextureManager::GetSingleton (void) {
	if (!m_Singleton) {
		m_Singleton = new NMS_TextureManager;
		Initialize ();
	}

	return *m_Singleton;
}

void NMS_TextureManager::Initialize (void) {
	sprintf (m_Singleton->szErrorMessage, "Texture Manager Initialized!");

	m_Singleton->nNumTextures = 0;
	m_Singleton->nAvailable   = INITIAL_SIZE;
	m_Singleton->nTexIDs      = new int [INITIAL_SIZE];
	
	for (int i = 0; i < m_Singleton->nAvailable; i++) {
		m_Singleton->nTexIDs [i] = -1;
	}
}

void NMS_TextureManager::Destroy (void) {
	if (m_Singleton) {
		delete [] m_Singleton->nTexIDs;
		m_Singleton->nTexIDs = 0;
	
		delete m_Singleton;
		m_Singleton = 0;
	}
}

// ===================================================================

int NMS_TextureManager::LoadTexture (const char *szFilename, int nTextureID) {
	 sprintf (m_Singleton->szErrorMessage, "Beginning to Loading [%s]", szFilename);
	 if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	  {
		/* wrong DevIL version */
		return -1;
	  }
	  ILuint texid=nTextureID;
	  GLuint image;
	  ILboolean success;
	  ilInit(); /* Initialization of DevIL */
	  ilGenImages(1, &texid); /* Generation of one image name */
	  ilBindImage(texid); /* Binding of image name */
	  success = ilLoadImage(szFilename); /* Loading of image "image.jpg" */  //USA iLoadImageF
	  if (success) /* If no error occured: */
	  {
		success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
		  unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
		if (!success)
		{
		  /* Error occured */
		  return -1;
		}
		glGenTextures(1, &image); /* Texture name generation */
		glBindTexture(GL_TEXTURE_2D, image); /* Binding of texture name */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for magnification filter */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); /* We will use linear
		  interpolation for minifying filter */
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
		  ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
		  ilGetData()); /* Texture specification */
	  }
	  else
	  {
		/* Error occured */
		return -1;
	  }
	  ilDeleteImages(1, &texid); 

	sprintf (m_Singleton->szErrorMessage, "Loaded [%s] W/O a hitch!", szFilename);
	return image;;
}

void NMS_TextureManager::FreeTexture (int nID) {
	int nIndex = -1;
	for (int i = 0; i < m_Singleton->nAvailable; i++) {
		if (m_Singleton->nTexIDs [i] == nID) {
			m_Singleton->nTexIDs [i] = -1;
			nIndex = i;	// to indicate a match was found
			break;		// their _should_ only be one instance of nID (if any)
		}
	}

	if (nIndex != -1) {
		unsigned int uiGLID = (unsigned int) nID;
		glDeleteTextures (1, &uiGLID);
	}
}

void NMS_TextureManager::FreeAll (void) {
	
	// copy the ids to an unsigned integer array, so GL will like it ;)
	unsigned int *pUIIDs = new unsigned int [m_Singleton->nNumTextures];
	int i, j;
	for (i = 0, j = 0; i < m_Singleton->nNumTextures; i++) {
		if (m_Singleton->nTexIDs [i] != -1) {
			pUIIDs [j] = m_Singleton->nTexIDs [i];
			j++;
		}
	}

	glDeleteTextures (m_Singleton->nNumTextures, pUIIDs);

	delete [] pUIIDs;
	delete [] m_Singleton->nTexIDs;
	m_Singleton->nTexIDs = new int [INITIAL_SIZE];
	m_Singleton->nAvailable = INITIAL_SIZE;
	for (i = 0; i < INITIAL_SIZE; i++)
		m_Singleton->nTexIDs [i] = -1;
	
	m_Singleton->nNumTextures = 0;
}

// ===================================================================

int NMS_TextureManager::GetNewTextureID (int nPossibleTextureID) {

	// First check if the possible textureID has already been
	// used, however the default value is -1, err that is what
	// this method is passed from LoadTexture ()
	if (nPossibleTextureID != -1) {
		for (int i = 0; i < m_Singleton->nAvailable; i++) {
			if (m_Singleton->nTexIDs [i] == nPossibleTextureID) {
				FreeTexture (nPossibleTextureID);	// sets nTexIDs [i] to -1...
				m_Singleton->nNumTextures--;		// since we will add the ID again...
				break;
			}
		}
	}

	// Actually look for a new one
	int nNewTextureID;
	if (nPossibleTextureID == -1) {
		unsigned int nGLID;	
		glGenTextures (1, &nGLID);
		nNewTextureID = (int) nGLID;
	}
	else	// If the user is handle the textureIDs
		nNewTextureID = nPossibleTextureID;
	
	// find an empty slot in the TexID array
	int nIndex = 0;
	while (m_Singleton->nTexIDs [nIndex] != -1 && nIndex < m_Singleton->nAvailable)
		nIndex++;

	// all space exaused, make MORE!
	if (nIndex >= m_Singleton->nAvailable) {
		int *pNewIDs = new int [m_Singleton->nAvailable + TEXTURE_STEP];
		int i;
		
		// copy the old
		for (i = 0; i < m_Singleton->nAvailable; i++)
			pNewIDs [i] = m_Singleton->nTexIDs [i];
		// set the last increment to the newest ID
		pNewIDs [m_Singleton->nAvailable] = nNewTextureID;
		// set the new to '-1'
		for (i = 1; i < TEXTURE_STEP; i++)
			pNewIDs [i + m_Singleton->nAvailable] = -1;

		m_Singleton->nAvailable += TEXTURE_STEP;
		delete [] m_Singleton->nTexIDs;
		m_Singleton->nTexIDs = pNewIDs;
	}
	else
		m_Singleton->nTexIDs [nIndex] = nNewTextureID;

	// Welcome to our Texture Array!
	m_Singleton->nNumTextures++;
	return nNewTextureID;
}

// ===================================================================

char *NMS_TextureManager::GetErrorMessage (void) {
	return m_Singleton->szErrorMessage;
}

bool NMS_TextureManager::CheckSize (int nDimension) {
	// Portability issue, check your endian...

	int i = 1;
	while (i < nDimension) {
		i <<= 1;
		if (i == nDimension)
			return true;
	}

	return false;
}

int	NMS_TextureManager::GetNumTextures (void) {
	return m_Singleton->nNumTextures;
}

int NMS_TextureManager::GetAvailableSpace (void) {
	return m_Singleton->nAvailable;
}

int NMS_TextureManager::GetTexID (int nIndex) {
	if (nIndex >= 0 && nIndex < m_Singleton->nAvailable)
		return m_Singleton->nTexIDs [nIndex];

	// else
	return 0;
}