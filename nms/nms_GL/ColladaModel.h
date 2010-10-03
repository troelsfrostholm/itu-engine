#ifdef __EXP_NMS_GL
#    define COLLADAMODEL_D __declspec(dllexport)
#else
#    define COLLADAMODEL_D __declspec(dllimport)
#endif

#ifndef __COLLADAMODEL_H__
#define __COLLADAMODEL_H__

#include <irrXML.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <irrTypes.h>
#include <irrString.h>
#include <fast_atof.h>
#include "NMS_FileManagement.h"
#include "NMS_AssetManager.h"
#include "NMS_Mesh.h"

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 

#pragma warning( disable: 4251 )  //Used to disable this useless warning: http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html

using namespace irr;
using namespace io;
using namespace std;

typedef float vec9_t[3][3];
typedef float vec6_t[3][2];


class COLLADAMODEL_D Source
{	
    public:
	  Source(){};
	  core::stringc sName;
	  core::stringc sID;

	  float* pfArray;
	  unsigned nElements;

	//Accessor variables
	  unsigned count;
	  unsigned offset;
	  unsigned stride;
};

class COLLADAMODEL_D Triangle
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

class COLLADAMODEL_D ColMesh
{	
  public:
	ColMesh();
	vector<Source> sources;
	vector<Triangle> triangles;
	core::stringc sVertPosition;
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

	
	//XML DATA ACQUISITION
	void readLibraryImages(IrrXMLReader* xml);
	void readLibraryGeometries(IrrXMLReader* xml);
	void readMainSection(IrrXMLReader* xml);
	void removeWhitespaces(char** start);
	vector<ColMesh> dataRead;
	string textureFilepath;
	int textureID;

	//RENDERING THE MODEL
	void    RenderFrame();
};
#endif