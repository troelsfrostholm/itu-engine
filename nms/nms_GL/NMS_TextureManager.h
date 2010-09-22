#pragma once

#include <IL/il.h>


/***
	NMS_TextureManager v1.00 [5/22/01]
		To get the initial bout of confusion out of the way
		this class is a singleton, IE their is only one
		instance in memory at any time; so you don't have
		to worry about the whole 'which object has a texture
		manager, which object needs one, how to get it ect
		ect'.

		The class works for the most part as a simple texture
		loader via ::LoadTexture () however, you can fee any
		textures you've loaded into memory via ::FreeTexture
		or even FreeAll ().  This is done by an array of every
		texture ID created by LoadTexture.  Note that with
		this class you can ither specify your own texture ID's
		or let OpenGL generate them for you (hence the return
		value from LoadTexture)
		
		To 'clean up' just call DESTROY_TEXMANAGER, this will
		delete the TexID array and call FreeAll.

	Usage :
		TEXMANAGER.LoadTexture ("Wall.bmp", WALL_TEXTURE);

		int nFloorID = TEXMANAGER.LoadTexture ("Floor.tga");
		if (nFloorID != 0)
			cout << TEXMANAGER.GetErrorMessage () << endl;

		TEXMANAGER.FreeTexture (WALL_TEXTURE);
		TEXManager.LoadTexture ("Wall2.bmp", WALL_TEXTURE);
		  
		TEXMANAGER.FreeAll ();

	In Conclusion :
		Well, if you find any bugs, err features!  Or ::gasp::
		even _like_ it, send me a line at
			Christopher.Smith@Trinity.edu
			www.cs.trinity.edu/~csmith8
***/
		
#define TEXMANAGER	NMS_TextureManager::GetSingleton()
#define DESTROY_TEXMANAGER	NMS_TextureManager::Destroy();

#define UBYTE unsigned char
#define INITIAL_SIZE	32	// initial size of the TexID array
#define TEXTURE_STEP	8	// how much the array grows by each time

class NMS_TextureManager {
public :
	NMS_TextureManager (void);
	~NMS_TextureManager (void);
	static NMS_TextureManager &GetSingleton (void);

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