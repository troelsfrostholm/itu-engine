#ifdef __EXP_NMS_GL
#    define MESHMANAGER_D __declspec(dllexport)
#else
#    define MESHMANAGER_D __declspec(dllimport)
#endif

#ifndef __NMS_MESHMANAGER
#define __NMS_MESHMANAGER

#pragma warning( disable: 4251 )  //Used to disable this useless warning: http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html

#include "NMS_sha1.h"
#include "NMS_FileManagement.h"
#include "NMS_LogFunctions.h"
#include "NMS_CustomTypes.h"
#include "NMS_StaticAllocator.h"
#include "NMS_LevelAllocator.h"
#include "NMS_Mesh.h"

#include <map>
#include <string.h>
#include <stdio.h>



#define NMS_MESHMANAGER	NMS_MeshManager::GetSingleton()
#define DESTROY_NMS_MESHMANAGER	NMS_MeshManager::Destroy()


//This structure contains the pointer to the mesh to be retrieved along with an hash related to the file that has been loaded
typedef struct
{
	NMS_Mesh* mesh;
	shaMap hash;
}meshStruct;

class MESHMANAGER_D NMS_MeshManager {
public :

	static NMS_MeshManager& GetSingleton (void);

private :
	NMS_MeshManager (void);
	~NMS_MeshManager (void);
	static void Initialize (void);
	textStruct checkForHash(shaMap hash,string textureName);

public :
	//Load a mesh
	int LoadMesh (const char *szFilename,char* textureName);
	//Release the memory space occupied by a specific mesh
	void FreeMesh (char* meshName);
	//Release the memory occupied by all the textures
	void FreeAll (void);
	static void Destroy (void);

private :
	static NMS_MeshManager* m_Singleton;
	char m_sMessage[300];
	static const size_t m_iMessageSize=300*sizeof(char);

	struct stringLess : binary_function <string, string, bool> {
		bool operator() (const string& x, const string& y) const
			{ return x.compare(y)<0; }
	};
	std::map<string, meshStruct, stringLess> meshMap;
};
#endif