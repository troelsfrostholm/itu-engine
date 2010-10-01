#include "ColladaModel.h"

using namespace irr;
using namespace io;
using namespace std;

ColladaModel::ColladaModel(){};
ColladaModel::~ColladaModel(){};

void 	ColladaModel::DrawModel(float time)
{
	glPushMatrix();
        // rotate the model
		glRotatef( -90.0, 1.0, 0.0, 0.0 );
        glRotatef( 180.0f, 0.0, 0.0, 1.0 );
		RenderFrame();
	glPopMatrix();
}

void 	ColladaModel::RenderFrame()
{
	unsigned i=0;
	glPushAttrib( GL_POLYGON_BIT );
    glFrontFace( GL_CW );
	
    // enable backface culling
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
	vec9_t vertices;
	unsigned vertOffset=ColladaModel::dataRead.back().iVertOffset;
	Source positionSource=Source();
	//Find the right source for the positions inside the sources vector
	for (i=0; i<dataRead.back().sources.size(); i++) {
		if(dataRead.back().sources[i].sID==dataRead.back().sVertPosition)
		{
			positionSource=dataRead.back().sources[i];
			break;
		}
	}
	unsigned firstOffset;
	unsigned secondOffset;
	unsigned thirdOffset;
	glBegin(GL_TRIANGLES);
	for(unsigned i=0;i<ColladaModel::dataRead.back().iTriangleCount;i++)
	{
		firstOffset=dataRead.back().pTriangleData[i*6+vertOffset];
		vertices[0]=positionSource.pfArray[firstOffset*3+positionSource.offset+0];
		vertices[1]=positionSource.pfArray[firstOffset*3+positionSource.offset+1];
		vertices[2]=positionSource.pfArray[firstOffset*3+positionSource.offset+2];
		secondOffset=dataRead.back().pTriangleData[i*6+vertOffset+2];
		vertices[3]=positionSource.pfArray[secondOffset*3+positionSource.offset+0];
		vertices[4]=positionSource.pfArray[secondOffset*3+positionSource.offset+1];
		vertices[5]=positionSource.pfArray[secondOffset*3+positionSource.offset+2];
		thirdOffset=dataRead.back().pTriangleData[i*6+vertOffset+4];
		vertices[6]=positionSource.pfArray[thirdOffset*3+positionSource.offset+0];
		vertices[7]=positionSource.pfArray[thirdOffset*3+positionSource.offset+1];
		vertices[8]=positionSource.pfArray[thirdOffset*3+positionSource.offset+2];
		glVertex3f(vertices[0],vertices[1],vertices[2]);
		glVertex3f(vertices[3],vertices[4],vertices[5]);
		glVertex3f(vertices[6],vertices[7],vertices[8]);
	}
	glEnd();
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
						dataRead.back().iTriangleCount=xml->getAttributeValueAsInt("count");
						dataRead.back().sMeshMaterial=xml->getAttributeValue("material");
						xml->read();
						//While we are still in the vertices section...
						while(strcmp("triangles",xml->getNodeName()))
						{
							//Check if we have an input node and it's related to the position
							if((!strcmp("input",xml->getNodeName()))&&(!strcmp("VERTEX",xml->getAttributeValue("semantic"))))
							{
								//Save the right array to get the position from
								dataRead.back().sVertSource=xml->getAttributeValue("source");
								dataRead.back().iVertOffset=xml->getAttributeValueAsInt("offset");
							}
							else
							if((!strcmp("input",xml->getNodeName()))&&(!strcmp("NORMAL",xml->getAttributeValue("semantic"))))
							{
								//Save the right array to get the position from
								dataRead.back().sNormSource=xml->getAttributeValue("source");
								dataRead.back().iNormOffset=xml->getAttributeValueAsInt("offset");
							}
							else
							if((!strcmp("input",xml->getNodeName()))&&(!strcmp("TEXCOORD",xml->getAttributeValue("semantic"))))
							{
								//Save the right array to get the position from
								dataRead.back().sTextSource=xml->getAttributeValue("source");
								dataRead.back().iTextOffset=xml->getAttributeValueAsInt("offset");
							}
							else
							if((!strcmp("p",xml->getNodeName()))&&(xml->getNodeType()!=EXN_ELEMENT_END))
							{
								xml->read();
								//Save the offset related to the positions in the arrays
							    char* charArray=(char*)xml->getNodeData();
								int maxTriangles=dataRead.back().iTriangleCount*9;
								dataRead.back().pTriangleData=new int[maxTriangles];
								int* tempArray=dataRead.back().pTriangleData;
								for (int i=0; i<maxTriangles; i++)
									tempArray[i]=strtol(charArray,&charArray,10);
							}
							xml->read();
							core::stringc name=xml->getNodeName();
						}
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
					float* tempArray=dataRead.back().sources.back().pfArray;
					for (int i=0; i<maxVertices; i++)
					{
						removeWhitespaces(&charArray);
						float fToBeConverted;
						//Copy a float into the array
						charArray = core::fast_atof_move(charArray, fToBeConverted);
						tempArray[i] = fToBeConverted;
					}
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
