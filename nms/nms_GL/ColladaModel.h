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

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library 

#pragma warning( disable: 4251 )  //Used to disable this useless warning: http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html

using namespace irr;
using namespace io;
using namespace std;

typedef float vec9_t[9];


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


class COLLADAMODEL_D ColMesh
{	
  public:
	ColMesh();
	vector<Source> sources;
	core::stringc sVertPosition;

	//Mesh data definitions
	core::stringc sMeshMaterial;


	core::stringc sVertSource;
	unsigned iVertOffset;
	core::stringc sNormSource;
	unsigned iNormOffset;
	core::stringc sTextSource;
	unsigned iTextOffset;



	//Triangles count
	unsigned iTriangleCount;
	int* pTriangleData;

	//Data to be loaded
	bool bVertices;
	bool bTextures;
	bool bNormals;
	unsigned uNumberOfData;
};

class COLLADAMODEL_D ColladaModel
{
public:
	ColladaModel();
	~ColladaModel();
	int		LoadModel(const char* fileName);
	int     LoadSkin(char* fileName);
	int     LoadSkin();
	void	DrawModel(float time);
	void	DrawFrame(int frame,int nFrame); // base zero
private:

	
	//XML DATA ACQUISITION
	void readLibraryImages(IrrXMLReader* xml);
	void readLibraryGeometries(IrrXMLReader* xml);
	void readMainSection(IrrXMLReader* xml);
	void removeWhitespaces(char** start);
	vector<ColMesh> dataRead;
	string textureFilepath;

	//RENDERING THE MODEL
	void    RenderFrame();
};
#endif