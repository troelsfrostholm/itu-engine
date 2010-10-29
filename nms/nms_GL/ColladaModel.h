#ifdef __EXP_NMS_GL
#    define COLLADAMODEL_D __declspec(dllexport)
#else
#    define COLLADAMODEL_D __declspec(dllimport)
#endif




#ifndef __COLLADAMODEL_H__
#define __COLLADAMODEL_H__

#include "NMS_FileManagement.h"
#include "NMS_AssetManager.h"
#include "NMS_Mesh.h"
#include "Matrix.h"

#include "SDL_opengl.h"
#include <irrXML.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <irrTypes.h>
#include <irrString.h>
#include <fast_atof.h>

#pragma warning( disable: 4251 )  //Used to disable this useless warning: http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html


using namespace irr;
using namespace io;
using namespace std;

typedef float vec9_t[9];
typedef float vec6_t[6];

//class Accessor
//{	
//  public:
//	Accessor(){iCount=0;iOffset=0;iStride=0;sSource="";};
//	unsigned iCount;
//	unsigned iOffset;
//	unsigned iStride;
//	core::stringc sSource;
//};

class Source
{	
    public:
	  Source(){sName=NULL;
			   sID=NULL;
			   accessorReference=NULL;
			   pIdRefArray=NULL;
			   iFArraySize=0;
			   iFArraySize=0;
			   sParameterType="";};
	  core::stringc sName;
	  core::stringc sID;

	  core::stringc sParameterType;

	  float* pfArray;
	  unsigned iFArraySize;
	  core::stringc* pIdRefArray;
	  unsigned iIdRefArraySize;
	  unsigned nElements;

	//Accessor variables
	  unsigned count;
	  unsigned offset;
	  unsigned stride;


	  /*vector<Accessor> vAccessor;*/
	  core::stringc accessorReference;
};


class Triangle
{	
    public:
	  Triangle();
	  core::stringc sVertSource;
	  core::stringc sNormSource;
	  core::stringc sTextSource;

	  //Mesh data definitions
	  core::stringc sTriangleMaterial;

	  unsigned iVertOffset;
	  unsigned iNormOffset;
	  unsigned iTextOffset;

	  //Triangles count
	  unsigned iTriangleCount;
	  int* pTriangleData;

	  unsigned iTriangleOffset;

	  //Data to be loaded
		bool bVertices;
		bool bTextures;
		bool bNormals;
		unsigned uNumberOfData;
	
};

class ColMesh
{	
  public:
	ColMesh();
	vector<Source> sources;
	vector<Triangle> triangles;
	core::stringc sVertPosition;
};

class Material
{	
  public:
	Material();
	core::stringc sID;
	core::stringc sName;
	core::stringc sUrl;
};





class Node
{	
  public:
	Node();
	core::stringc sID;
	core::stringc sName;
	core::stringc sSID;
	core::stringc sType;
	std::map<core::stringc ,Node> nodes;
	Matrix transformation;
};

class Effect
{	
  public:
	Effect();
	core::stringc sID;
	core::stringc sName;
	core::stringc sSurface;
};

class Image
{	
  public:
	Image();
	core::stringc sID;
	core::stringc sName;
	core::stringc sPath;
};

class Skin
{
public: 
	Skin(){iWeightCount=0;};
	Matrix mBindShape;
	vector<Source> vSources;

	core::stringc jointSource;
	core::stringc bindSource;

	unsigned iWeightCount;
	unsigned iVCount;
	unsigned iJointOffset;
	unsigned iWeightOffset;
	unsigned* pVCount;
	unsigned* pV;
};

class RenderData
{
   public:
	 RenderData();
     unsigned iTextID;

	 //Triangles count
	 unsigned iTriangleCount;
	 float* vVertices;
	 float* vTextures;
	 float* vNormals;
};


class COLLADAMODEL_D ColladaModel : public NMS_Mesh
{
public:
	ColladaModel();
	~ColladaModel();
	int		LoadModel(const char* fileName);
	int     LoadSkin(char* fileName);
	int     LoadSkin();
	void	render(float time);
	void	DrawFrame(int frame,int nFrame); // base zero
private:

	//Check to avoid crashes: the model has loaded in the right way
	bool                bModelLoadedCorrectly;
	bool				bXMLLoaded;
	unsigned			iTriangleCount;
	unsigned			iMeshCount;

	Skin				skinningInformation;
	
	vector<ColMesh>     dataRead;
	vector<Material>    vMaterials;
	vector<Effect>	    vEffects;
	vector<Image>       vImages;
	vector<RenderData>  vRenderData;
	std::map<core::stringc ,Node>   mNodes;

	//Transformation for the scene
	Matrix transformation;

	core::stringc sSkeletonID;
	Node* pSkeletonNode;


	
	//XML DATA ACQUISITION
	void readLibraryImages(IrrXMLReader* xml);
	void readLibraryGeometries(IrrXMLReader* xml);
	void readLibraryMaterials(IrrXMLReader* xml);
	void readLibraryEffects(IrrXMLReader* xml);
	void readMainSection(IrrXMLReader* xml);
	void readLibraryVisualScene(IrrXMLReader* xml);
	void readLibraryControllers(IrrXMLReader* xml);
	void readController(IrrXMLReader* xml);
	void readSkin(IrrXMLReader* xml);
	void readJoint(IrrXMLReader* xml);
	void readVertexWeight(IrrXMLReader* xml);
	
	Source readSource(IrrXMLReader* xml);
	void RenderFrame();

	//XML TRANSFORMATION READING
	Matrix readMatrix(IrrXMLReader* xml);
	Matrix readTranslation(IrrXMLReader* xml);
	Matrix readRotation(IrrXMLReader* xml);
	Matrix readScale(IrrXMLReader* xml);

	//DATA READING
	void readFloatArray(IrrXMLReader* xml,float* arrayPointer);
	void readIDREFArray(IrrXMLReader* xml,core::stringc* arrayPointer);
	void readVCountArray(IrrXMLReader* xml,unsigned* arrayPointer);
	void readVArray(IrrXMLReader* xml,unsigned* arrayPointer);

	void   readNode(IrrXMLReader* xml,Node* parent);
	void   readInstanceController(IrrXMLReader* xml);

	//RENDERING THE MODEL
	void    LoadData();
};
#endif


