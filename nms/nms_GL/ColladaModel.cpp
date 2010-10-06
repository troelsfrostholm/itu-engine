#include "ColladaModel.h"

using namespace irr;
using namespace io;
using namespace std;

ColladaModel::ColladaModel()
{
	textureID=0;
	bModelLoadedCorrectly   =false;
};

ColladaModel::~ColladaModel(){};


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

void ColladaModel::render(float time)
{
	if(bModelLoadedCorrectly)
	{
		glPushMatrix();
			if(textureID==0)
				LoadSkin((char*)textureFilepath.c_str());
			RenderFrame();
		glPopMatrix();
	}
	else
	{
		LOG.write("MD2Model::DrawModel -> The model has not been loaded correctly cannot draw it!\n",LOG_ERROR);
	}
}

int ColladaModel::LoadSkin(char *filename)
{
	textureID=NMS_ASSETMANAGER.LoadTexture(filename,filename);
	return textureID;
}

void 	ColladaModel::RenderFrame()
{
	unsigned i=0;
	unsigned t=0;
	unsigned m=0;
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, textureID );
	//FOR EACH MESH
	for(m=0;m<dataRead.size();m++)
	{
		vec9_t vertices;
		vec6_t textures;
		
		Source* positionSource;
		Source* textureSource;
		//Find the right source for the positions inside the sources vector
		unsigned firstOffset;
		unsigned secondOffset;
		unsigned thirdOffset;
		//For each triangle declaration
		for(t=0;t<dataRead[m].triangles.size();t++)
		{
			   unsigned numberOfArrays=dataRead[m].triangles[t].uNumberOfData;
			   unsigned vertOffset=dataRead[m].triangles[t].iVertOffset;
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
				}
				unsigned numberOfTriangles=dataRead[m].triangles[t].iTriangleCount;
				unsigned vertexOffset=dataRead[m].triangles[t].iVertOffset;
				unsigned textureOffset=dataRead[m].triangles[t].iTextOffset;

				unsigned vertexStride=(*positionSource).stride;
				unsigned textureStride=(*textureSource).stride;

				GLfloat* textArray=(*textureSource).pfArray;
				bool   textEnabled=dataRead[m].triangles[t].bTextures;
				glBegin(GL_TRIANGLES);
				for(i=0;i<numberOfTriangles;i++)
				{
					if(textEnabled)
					{
						firstOffset=dataPointer[i*numberOfArrays*3+textureOffset];
						firstOffset=firstOffset*textureStride;
						textures[0][0]=textArray[firstOffset];
						textures[0][1]=textArray[firstOffset+1];
						secondOffset=dataPointer[i*numberOfArrays*3+textureOffset+numberOfArrays];
						secondOffset=secondOffset*textureStride;
						textures[1][0]=textArray[secondOffset];
						textures[1][1]=textArray[secondOffset+1];
						thirdOffset=dataPointer[i*numberOfArrays*3+textureOffset+numberOfArrays*2];
						thirdOffset=thirdOffset*textureStride;
						textures[2][0]=textArray[thirdOffset];
						textures[2][1]=textArray[thirdOffset+1];
					}
					firstOffset=dataPointer[i*numberOfArrays*3+vertOffset];
					firstOffset=firstOffset*vertexStride;
					vertices[0][0]=vertArray[firstOffset];
					vertices[0][1]=vertArray[firstOffset+1];
					vertices[0][2]=vertArray[firstOffset+2];
					secondOffset=dataPointer[i*numberOfArrays*3+vertOffset+numberOfArrays];
					secondOffset=secondOffset*vertexStride;
					vertices[1][0]=vertArray[secondOffset];
					vertices[1][1]=vertArray[secondOffset+1];
					vertices[1][2]=vertArray[secondOffset+2];
					thirdOffset=dataPointer[i*numberOfArrays*3+vertOffset+numberOfArrays*2];
					thirdOffset=thirdOffset*vertexStride;
					vertices[2][0]=vertArray[thirdOffset];
					vertices[2][1]=vertArray[thirdOffset+1];
					vertices[2][2]=vertArray[thirdOffset+2];
					glTexCoord2fv(textures[0]);
					glVertex3fv(vertices[0]);
					glTexCoord2fv(textures[1]);
					glVertex3fv(vertices[1]);
					glTexCoord2fv(textures[2]);
					glVertex3fv(vertices[2]);
				}
				glEnd();
		glDisable(GL_TEXTURE_2D);
		}
	}
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
		bModelLoadedCorrectly=true;
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
					while(xml->read())
					{
						switch(xml->getNodeType())
						{
							case EXN_ELEMENT:
							{
								if (!strcmp("init_from", xml->getNodeName()))
								{
									xml->read();//Read the filepath
									textureFilepath = xml->getNodeData();
									return;
								}
							}
						}
					}
				}
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
					float fToBeConverted;
					for (int i=0; i<maxVertices; i++)
									tempArray[i]=(GLfloat)strtod(charArray,&charArray);
					nextIsArray = false;
				}
			}
			break;
		case EXN_ELEMENT_END:
		{
			if (!strcmp("geometry",xml->getNodeName()))
			{
				//We have finished to read data from the geometry section
				return;
			}
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


void ColladaModel::removeWhitespaces(char** start)
{
	char* p = *start;

	while(*p && (*p==' ' || *p=='\n' || *p=='\r' || *p=='\t'))
		++p;

	*start = p;
}
