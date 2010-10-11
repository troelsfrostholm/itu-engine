#include "ColladaModel.h"

using namespace irr;
using namespace io;
using namespace std;

ColladaModel::ColladaModel()
{
	bModelLoadedCorrectly   =false;
	bXMLLoaded				=false;
	iTriangleCount          =0;
	iMeshCount				=0;
};

ColladaModel::~ColladaModel(){};




RenderData::RenderData()
{
	 iTextID=0;

	 //Triangles count
	 iTriangleCount=0;
}


Triangle::Triangle()
{
	  iNormOffset=0;
	  iTextOffset=0;
	  iVertOffset=0;
	  sNormSource=NULL;
	  sTextSource=NULL;
	  sVertSource=NULL;
	  sTriangleMaterial=NULL;
	  iTriangleCount=0;
	  pTriangleData=NULL;
	  iTriangleCount=0;
	  bVertices=false;
	  bTextures=false;
	  bNormals=false;
	  uNumberOfData=0;
}
ColMesh::ColMesh()
{
	sVertPosition=NULL;
}

Material::Material()
{	
	 sID=NULL;
	 sName=NULL;
	 sUrl=NULL;
};

Effect::Effect()
{	
	sID=NULL;
	sName=NULL;
	sSurface=NULL;
};

Image::Image()
{	
	
	sID=NULL;
	sName=NULL;
	sPath=NULL;
};


void ColladaModel::render(float time)
{
	if(bXMLLoaded)
	{
		if(bModelLoadedCorrectly)
		{
			glPushMatrix();
				RenderFrame();
			glPopMatrix();
		}
		else
		{
			LoadData();
		}
	}
	else
	{
		LOG.write("MD2Model::DrawModel -> The model has not been loaded correctly cannot draw it!\n",LOG_ERROR);
	}
}

int ColladaModel::LoadSkin(char *filename)
{
	return NMS_ASSETMANAGER.LoadTexture(filename,filename);
}

void ColladaModel::RenderFrame()
{
	glEnable(GL_TEXTURE_2D);
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	for(unsigned i=0;i<vRenderData.size();i++)
	{
		glEnableClientState( GL_VERTEX_ARRAY ); // Enable Vertex Arrays
		glEnableClientState( GL_TEXTURE_COORD_ARRAY ); // Enable Texture Coord Arrays
		glEnableClientState	(GL_NORMAL_ARRAY);
		glTexCoordPointer( 2, GL_FLOAT, 0, vRenderData[i].vTextures ); // Set The Vertex Pointer To TexCoord Data
		glNormalPointer(GL_FLOAT,0,vRenderData[i].vNormals);
		glVertexPointer( 3, GL_FLOAT, 0, vRenderData[i].vVertices ); // Set The Vertex Pointer To Vertex Data
		glDrawArrays( GL_TRIANGLES, 0, vRenderData[i].iTriangleCount*3 ); //Draw the vertices
	}
	glDisableClientState(GL_VERTEX_ARRAY); // Enable Vertex Arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY); // Enable Texture Coord Arrays
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable( GL_CULL_FACE );
	
}

void ColladaModel::LoadData()
{
	unsigned d=0;  //Misc count
	unsigned i=0;  //Image count
	unsigned t=0;  //Triangles count
	unsigned m=0;  //Mesh count
	iMeshCount=dataRead.size();
	//FOR EACH MESH
	for(m=0;m<dataRead.size();m++)
	{
		RenderData toBeRendered=RenderData();
		vec9_t vertices;
		vec9_t normals;
		vec6_t textures;
		
		Source* positionSource;
		Source* textureSource;
		Source* normalSource;
		//Find the right source for the positions inside the sources vector
		unsigned firstOffset;
		unsigned secondOffset;
		unsigned thirdOffset;
		//For each triangle declaration
		for(t=0;t<dataRead[m].triangles.size();t++)
		{
			   unsigned numberOfArrays=dataRead[m].triangles[t].uNumberOfData;
			   for (i=0; i<dataRead[m].sources.size(); i++) {
					if(dataRead[m].sources[i].sID==dataRead[m].sVertPosition)
					{
						positionSource=&dataRead[m].sources[i];
					}
					else
					if(dataRead[m].sources[i].sID==dataRead[m].triangles[t].sTextSource)
					{
						textureSource=&dataRead[m].sources[i];
					}
					if(dataRead[m].sources[i].sID==dataRead[m].triangles[t].sNormSource)
					{
						normalSource=&dataRead[m].sources[i];
					}
				}
				unsigned numberOfTriangles=dataRead[m].triangles[t].iTriangleCount;
				toBeRendered.iTriangleCount=numberOfTriangles;
				iTriangleCount+=numberOfTriangles;
				unsigned vertexOffset=dataRead[m].triangles[t].iVertOffset;
				unsigned textureOffset=dataRead[m].triangles[t].iTextOffset;
				unsigned normalOffset=dataRead[m].triangles[t].iNormOffset;

				unsigned vertexStride=(*positionSource).stride;
				unsigned textureStride=(*textureSource).stride;
				unsigned normalStride=(*normalSource).stride;
				
				int* dataPointer=dataRead[m].triangles[t].pTriangleData;
				GLfloat* vertArray=(*positionSource).pfArray;
				GLfloat* textArray=(*textureSource).pfArray;
				GLfloat* normArray=(*normalSource).pfArray;
				bool   textEnabled=dataRead[m].triangles[t].bTextures;
					int iTextureID=0;
					core::stringc temp=NULL;
					//Retrieve the material for the model
					for (d=0; d<vMaterials.size(); d++)
					{
						if(vMaterials[d].sName==dataRead[m].triangles[t].sTriangleMaterial)
						{
							//We have found the effect
							temp=vMaterials[d].sUrl;
							break;
						}
					}

					//Retrieve the effect
					for (d=0; d<vEffects.size(); d++)
					{
						if(vEffects[d].sName==temp)
						{
							//We have found the effect
							temp=vEffects[d].sSurface;
							break;
						}
					}

					//Retrieve the texture
					for (d=0; d<vImages.size(); d++)
					{
						if(vImages[d].sName==temp)
						{
							core::stringc image=vImages[d].sPath;
							int pos=image.findFirst('/');
							image=image.subString(pos+1,image.size());
							//We have found the texture, load it
							iTextureID=LoadSkin((char*)image.c_str());
							break;
						}
					}
					toBeRendered.vTextures=new GLfloat[numberOfTriangles*6];
					toBeRendered.vNormals=new GLfloat[numberOfTriangles*9];
					toBeRendered.vVertices=new GLfloat[numberOfTriangles*9];
					for(i=0;i<numberOfTriangles;i++)
					{
						if(textEnabled)
						{
							firstOffset=dataPointer[i*numberOfArrays*3+textureOffset];
							firstOffset=firstOffset*textureStride;
							toBeRendered.vTextures[0+i*6]=textArray[firstOffset];
							toBeRendered.vTextures[1+i*6]=textArray[firstOffset+1];
							secondOffset=dataPointer[i*numberOfArrays*3+textureOffset+numberOfArrays];
							secondOffset=secondOffset*textureStride;
							toBeRendered.vTextures[2+i*6]=textArray[secondOffset];
							toBeRendered.vTextures[3+i*6]=textArray[secondOffset+1];
							thirdOffset=dataPointer[i*numberOfArrays*3+textureOffset+numberOfArrays*2];
							thirdOffset=thirdOffset*textureStride;
							toBeRendered.vTextures[4+i*6]=textArray[thirdOffset];
							toBeRendered.vTextures[5+i*6]=textArray[thirdOffset+1];
						}

						//Normals loading
						firstOffset=dataPointer[i*numberOfArrays*3+normalOffset];
						firstOffset=firstOffset*normalStride;
						toBeRendered.vNormals[0+i*9]=normArray[firstOffset];
						toBeRendered.vNormals[1+i*9]=normArray[firstOffset+1];
						toBeRendered.vNormals[2+i*9]=normArray[firstOffset+2];
						secondOffset=dataPointer[i*numberOfArrays*3+normalOffset+numberOfArrays];
						secondOffset=secondOffset*normalStride;
						toBeRendered.vNormals[3+i*9]=normArray[secondOffset];
						toBeRendered.vNormals[4+i*9]=normArray[secondOffset+1];
						toBeRendered.vNormals[5+i*9]=normArray[secondOffset+2];
						thirdOffset=dataPointer[i*numberOfArrays*3+normalOffset+numberOfArrays*2];
						thirdOffset=thirdOffset*normalStride;
						toBeRendered.vNormals[6+i*9]=normArray[thirdOffset];
						toBeRendered.vNormals[7+i*9]=normArray[thirdOffset+1];
						toBeRendered.vNormals[8+i*9]=normArray[thirdOffset+2];

						//Vertices loading
						firstOffset=dataPointer[i*numberOfArrays*3+vertexOffset];
						firstOffset=firstOffset*vertexStride;
						toBeRendered.vVertices[0+i*9]=vertArray[firstOffset];
						toBeRendered.vVertices[1+i*9]=vertArray[firstOffset+1];
						toBeRendered.vVertices[2+i*9]=vertArray[firstOffset+2];
						secondOffset=dataPointer[i*numberOfArrays*3+vertexOffset+numberOfArrays];
						secondOffset=secondOffset*vertexStride;
						toBeRendered.vVertices[3+i*9]=vertArray[secondOffset];
						toBeRendered.vVertices[4+i*9]=vertArray[secondOffset+1];
						toBeRendered.vVertices[5+i*9]=vertArray[secondOffset+2];
						thirdOffset=dataPointer[i*numberOfArrays*3+vertexOffset+numberOfArrays*2];
						thirdOffset=thirdOffset*vertexStride;
						toBeRendered.vVertices[6+i*9]=vertArray[thirdOffset];
						toBeRendered.vVertices[7+i*9]=vertArray[thirdOffset+1];
						toBeRendered.vVertices[8+i*9]=vertArray[thirdOffset+2];
						//Store the loaded data in the main class
					}
		 toBeRendered.iTextID=iTextureID;
		 vRenderData.push_back(toBeRendered);
		}
		

	}
	bModelLoadedCorrectly=true;
}


int	ColladaModel::LoadModel(const char* fileName)
{
	IrrXMLReader* xml = createIrrXMLReader(fileName);
	if(nmsFileManagement::FileExists(fileName))
	{
		// parse the file until end reached
		while(xml && xml->read())
		{
			switch(xml->getNodeType())
			{
				case EXN_ELEMENT:
				{
					if (!strcmp("COLLADA", xml->getNodeName()))  //REMEMBER! Strcmp return 0 on success
					{
						readMainSection(xml);
					}
					else
					{
						//It's not a collada file, return 0
						return 1;
					}
				}
				break;
			}
		}
		// delete the xml parser after usage
		delete xml;
		
		bXMLLoaded=true;
		return true;
	}
	else
		return false;
	
}


void ColladaModel::readLibraryImages(IrrXMLReader* xml)
{
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("image", xml->getNodeName()))
				{
					Image cImage=Image(); 
					cImage.sID=xml->getAttributeValue("id");
					cImage.sName=xml->getAttributeValue("name");
					bool imageEnd=false;
					while(xml->read()&&!imageEnd)
					{
						switch(xml->getNodeType())
						{
							case EXN_ELEMENT:
							{
								if (!strcmp("init_from", xml->getNodeName()))
								{
									xml->read();
									cImage.sPath=xml->getNodeData();
								}
							}break;
							case EXN_ELEMENT_END:
							{
								if (!strcmp("init_from", xml->getNodeName()))
									imageEnd=true;
							}break;
						}
					}
					vImages.push_back(cImage);
				}
			}break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("image", xml->getNodeName()))
					return;
			}break;
		}
	}
}





void ColladaModel::readLibraryMaterials(IrrXMLReader* xml)
{
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("material", xml->getNodeName()))
				{
					Material cMaterial= Material();
					cMaterial.sID=xml->getAttributeValue("id");
					cMaterial.sName=xml->getAttributeValue("name");
					bool instanceEnd=false;
					while(xml->read()&&!instanceEnd)
					{
						switch(xml->getNodeType())
						{
							case EXN_ELEMENT:
							{
								if (!strcmp("instance_effect", xml->getNodeName()))
								{
									cMaterial.sUrl=xml->getAttributeValue("url");
									cMaterial.sUrl.replace('#',' ');
									cMaterial.sUrl.trim();
								}
							}break;
							case EXN_ELEMENT_END:
							{
								if (!strcmp("material", xml->getNodeName()))
									instanceEnd=true;
							}break;
						}
					}
					vMaterials.push_back(cMaterial);
				}
			}break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("library_materials", xml->getNodeName()))
					return;
			}break;
		}
	}
}

void ColladaModel::readLibraryEffects(IrrXMLReader* xml)
{
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("effect", xml->getNodeName()))
				{
					Effect cEffect= Effect();
					cEffect.sID=xml->getAttributeValue("id");
					cEffect.sName=xml->getAttributeValue("name");
					bool effectEnded=false;
					while(xml->read()&&!effectEnded)
					{
						switch(xml->getNodeType())
						{
							case EXN_ELEMENT:
								{
									if (!strcmp("surface", xml->getNodeName()))
									{
										bool surfaceFound=false;
										while(xml->read()&&!surfaceFound)
										{
											switch(xml->getNodeType())
											{
												case EXN_ELEMENT:
												{
													if (!strcmp("init_from", xml->getNodeName()))
													{
														xml->read();
														cEffect.sSurface=xml->getNodeData();
													}
												}break;
												case EXN_ELEMENT_END:
												{
													if (!strcmp("surface", xml->getNodeName()))
														surfaceFound=true;
												}break;
											}
										}
										vEffects.push_back(cEffect);
									}
								}break;
							case EXN_ELEMENT_END:
								{
									if (!strcmp("profile_COMMON", xml->getNodeName()))
									{
										effectEnded=true;
									}
								}
						}
					}
				}
			}break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("library_effects", xml->getNodeName()))
					return;
			}break;
		}
	}
}




void ColladaModel::readLibraryGeometries(IrrXMLReader* xml)
{
	bool nextIsArray = false;

	// read sources with arrays and accessor for each mesh

	if (!xml->isEmptyElement())
	while(xml->read())
	{
		const char* nodeName=xml->getNodeName();
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
				{
					if (!strcmp("geometry",xml->getNodeName()))
					{
						ColMesh mesh =  ColMesh();
						dataRead.push_back(mesh);
						break;
					}
					else
					if (!strcmp("mesh",xml->getNodeName()))
					{
						//Skip it and do nothing, nothing to read, nothing important
					}
					else
					if (!strcmp("source",xml->getNodeName()))
					{
						Source source =  Source();
						source.sName = xml->getAttributeValue("name");
						source.sID = xml->getAttributeValue("id");
						dataRead.back().sources.push_back(source);
						break;
					}
					else
					if (!strcmp("float_array",xml->getNodeName()))
					{
							dataRead.back().sources.back().nElements=xml->getAttributeValueAsInt("count");
							nextIsArray=true;
							break;
					}
					else
					if (!strcmp("accessor",xml->getNodeName()))
					{
						
						dataRead.back().sources.back().count  = xml->getAttributeValueAsInt("count");
						dataRead.back().sources.back().offset = xml->getAttributeValueAsInt("offset");
						dataRead.back().sources.back().stride = xml->getAttributeValueAsInt("stride");
						if (dataRead.back().sources.back().stride == 0)
							dataRead.back().sources.back().stride = 1;   //Fix a bug in some models
						break;
					}
					else
					if (!strcmp("vertices",xml->getNodeName()))
					{
						xml->read();
						//While we are still in the vertices section...
						while(strcmp("vertices",xml->getNodeName()))
						{
							//Check if we have an input node and it's related to the position
							if((!strcmp("input",xml->getNodeName()))&&(!strcmp("POSITION",xml->getAttributeValue("semantic"))))
							{
								//Save the right array to get the position from
								dataRead.back().sVertPosition=xml->getAttributeValue("source");
								//Remove the # character
								dataRead.back().sVertPosition.replace('#',' ');
								dataRead.back().sVertPosition.trim();
							}
							xml->read();
						}
					}
					if (!strcmp("triangles",xml->getNodeName()))
					{
						Triangle triangle =  Triangle();
						triangle.iTriangleCount=xml->getAttributeValueAsInt("count");
						triangle.sTriangleMaterial=xml->getAttributeValue("material");
						xml->read();
						int toBeRetrieved=0;
						//While we are still in the vertices section...
						while(strcmp("triangles",xml->getNodeName()))
						{
							//Check if we have an input node and it's related to the position
							if((!strcmp("input",xml->getNodeName()))&&(!strcmp("VERTEX",xml->getAttributeValue("semantic"))))
							{
								//Save the right array to get the position from
								triangle.sVertSource=xml->getAttributeValue("source");
								triangle.sVertSource.replace('#',' ');
								triangle.sVertSource.trim();
								triangle.iVertOffset=xml->getAttributeValueAsInt("offset");
								triangle.bVertices=true;
								triangle.uNumberOfData++;
								toBeRetrieved=3;
							}
							else
							if((!strcmp("input",xml->getNodeName()))&&(!strcmp("NORMAL",xml->getAttributeValue("semantic"))))
							{
								//Save the right array to get the position from
								triangle.sNormSource=xml->getAttributeValue("source");
								triangle.sNormSource.replace('#',' ');
								triangle.sNormSource.trim();
								triangle.iNormOffset=xml->getAttributeValueAsInt("offset");
								triangle.bNormals=true;
								triangle.uNumberOfData++;
								toBeRetrieved=3;
							}
							else
							if((!strcmp("input",xml->getNodeName()))&&(!strcmp("TEXCOORD",xml->getAttributeValue("semantic"))))
							{
								//Save the right array to get the position from
								triangle.sTextSource=xml->getAttributeValue("source");
								//Remove the # character
								triangle.sTextSource.replace('#',' ');
								triangle.sTextSource.trim();
								triangle.iTextOffset=xml->getAttributeValueAsInt("offset");
								triangle.bTextures=true;
								triangle.uNumberOfData++;
								toBeRetrieved=2;
							}
							else
							if((!strcmp("p",xml->getNodeName()))&&(xml->getNodeType()!=EXN_ELEMENT_END))
							{
								xml->read();
								//Save the offset related to the positions in the arrays
							    char* charArray=(char*)xml->getNodeData();
								int maxTriangles=triangle.iTriangleCount*3*triangle.uNumberOfData;
								triangle.pTriangleData=new int[maxTriangles];
								int* tempArray=triangle.pTriangleData;
								for (int i=0; i<maxTriangles; i++)
									tempArray[i]=strtol(charArray,&charArray,10);
							}
							xml->read();
							core::stringc name=xml->getNodeName();
						}
						dataRead.back().triangles.push_back(triangle);
					}
				} 
				break;
		case EXN_TEXT:
			{
				if (nextIsArray)
				{
					char* charArray=(char*)xml->getNodeData();
					int maxVertices=dataRead.back().sources.back().nElements;
					dataRead.back().sources.back().pfArray=new float[maxVertices];
					GLfloat* tempArray=dataRead.back().sources.back().pfArray;
					for (int i=0; i<maxVertices; i++)
						tempArray[i]=(GLfloat)strtod(charArray,&charArray);
					nextIsArray = false;
				}
			}
			break;
		case EXN_ELEMENT_END:
		{
		}
		break;
	} 
  }
}





void ColladaModel::readMainSection(IrrXMLReader* xml)
{
	if (xml->isEmptyElement())
		return;

	// I ignore version information here. Keep on reading content:
	while(xml->read())
	{
		const char* nodeName=xml->getNodeName();
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("library_images", xml->getNodeName()))
				{
					//Read image informations
					readLibraryImages(xml);
				}
				else
				if (!strcmp("library_materials", xml->getNodeName()))
				{
					//Read image informations
					readLibraryMaterials(xml);
				}
				else
				if (!strcmp("library_effects", xml->getNodeName()))
				{
					//Read image informations
					readLibraryEffects(xml);
				}
				else
				if (!strcmp("library_geometries", xml->getNodeName()))
				{
					readLibraryGeometries(xml);
				}
				break;
			}
			case EXN_ELEMENT_END:
			{
				break; //End reading
			}
		}
	}
}