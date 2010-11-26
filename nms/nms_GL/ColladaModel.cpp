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
	transformation          =Matrix();
	sSkeletonID				=NULL;
	skinningInformation     =Skin();
	sSkeletonID			="";
};

ColladaModel::~ColladaModel(){};


Vertex::Vertex()
{
	vPosition=NULL;
	vNormals=NULL;
	vTextures=NULL;
}

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
Mesh::Mesh()
{
	sVertPosition=NULL;
}

Material::Material()
{	
	 sID=NULL;
	 sName=NULL;
	 sUrl=NULL;
};

Node::Node()
{	
	 sID=NULL;
	 sName=NULL;
	 sSID=NULL;
	 sType=NULL;
	 transformation=Matrix();
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
				DrawSkeleton();
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
	glEnableClientState( GL_VERTEX_ARRAY ); // Enable Vertex Arrays
	glEnableClientState( GL_TEXTURE_COORD_ARRAY ); // Enable Texture Coord Arrays
	glEnableClientState	(GL_NORMAL_ARRAY);
	for(unsigned i=0;i<vRenderData.size();i++)
	{
		float* vTextures= new float[vRenderData[i].iTriangleCount*6];
		float* vNormals= new float[vRenderData[i].iTriangleCount*9];
		float* vVertices= new float[vRenderData[i].iTriangleCount*9];

		//Now convert the pointers to the modified data to something that we can pass to 
		//OpenGL for bulk rendering
		for(unsigned k=0;k<vRenderData[i].iTriangleCount*3;k++)
		{
			vTextures[k*2]=*vRenderData[i].vTextures[k*2];
			vTextures[k*2+1]=*vRenderData[i].vTextures[k*2+1];

			vNormals[k*3]=*vRenderData[i].vNormals[k*3];
			vNormals[k*3+1]=*vRenderData[i].vNormals[k*3+1];
			vNormals[k*3+2]=*vRenderData[i].vNormals[k*3+2];

			vVertices[k*3]=*vRenderData[i].vVertices[k*3];
			vVertices[k*3+1]=*vRenderData[i].vVertices[k*3+1];
			vVertices[k*3+2]=*vRenderData[i].vVertices[k*3+2];
		}

		glTexCoordPointer( 2, GL_FLOAT, 0, vTextures ); // Set The Vertex Pointer To TexCoord Data
		glNormalPointer(GL_FLOAT,0, vNormals);
		glVertexPointer( 3, GL_FLOAT, 0, vVertices); // Set The Vertex Pointer To Vertex Data
		glDrawArrays( GL_TRIANGLES, 0, vRenderData[i].iTriangleCount*3); //Draw the vertices
		//delete(vTextures);
		//delete(vNormals);
		//delete(vVertices);
	}
	glDisableClientState(GL_VERTEX_ARRAY); // Enable Vertex Arrays
	glDisableClientState(GL_TEXTURE_COORD_ARRAY); // Enable Texture Coord Arrays
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisable(GL_TEXTURE_2D);
	glDisable( GL_CULL_FACE );
}

void ColladaModel::LoadData()
{
unsigned d=0; //Misc count
unsigned i=0; //Image count
unsigned t=0; //Triangles count
unsigned m=0; //Mesh count
iMeshCount=dataRead.size();
int maxVertices=dataRead.back().sources[dataRead.back().sVertPosition].iFArraySize/3;
pVertArray = new Vertex[maxVertices];

//FOR EACH MESH
for(m=0;m<dataRead.size();m++)
{
				RenderData toBeRendered=RenderData();

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
							positionSource=&dataRead[m].sources[dataRead[m].sVertPosition];
							textureSource=&dataRead[m].sources[dataRead[m].triangles[t].sTextSource];
							normalSource=&dataRead[m].sources[dataRead[m].triangles[t].sNormSource];
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
							float* vertArray=positionSource->pfArray;
							float* textArray=textureSource->pfArray;
							float* normArray=normalSource->pfArray;

							bool textEnabled=dataRead[m].triangles[t].bTextures;
							unsigned iTextureID=0;
							core::stringc temp=NULL;

							//Retrieve the material for the model
							for (d=0; d<vMaterials.size(); d++)
							{
								if(vMaterials[d].sID==dataRead[m].triangles[t].sTriangleMaterial||vMaterials[d].sName==dataRead[m].triangles[t].sTriangleMaterial)
								{
									//We have found the material
									temp=vMaterials[d].sUrl;
									break;
								}
							}

							//Retrieve the effect
							for (d=0; d<vEffects.size(); d++)
							{
								if(vEffects[d].sID==temp||vEffects[d].sName==temp)
								{
									//We have found the effect
									temp=vEffects[d].sSurface;
									break;
								}
							}

							//Retrieve the texture
							for (d=0; d<vImages.size(); d++)
							{
								if(vImages[d].sID==temp||vImages[d].sName==temp)
								{
									core::stringc image=vImages[d].sPath;
									int pos=image.findFirst('/');
									image=image.subString(pos+1,image.size());
									//We have found the texture, load it
									iTextureID=LoadSkin((char*)image.c_str());
									break;
								}
							}

							toBeRendered.vTextures=new(LEVEL_ALLOC, MEM_LEVEL) float*[numberOfTriangles*6];
							toBeRendered.vNormals=new(LEVEL_ALLOC, MEM_LEVEL) float*[numberOfTriangles*9];
							toBeRendered.vVertices=new(LEVEL_ALLOC, MEM_LEVEL) float*[numberOfTriangles*9];
							int firstVertex;
							int secondVertex;
							int thirdVertex;



							//PVert array is a structure that contain data for each vertex.
							//It contains the position, the normals and the texture coordinate for each vertex.
							//toBeRendered contains references like the offset in the collada file in a way that we can
							//modify just PVert array in the skinning part and being able to get the modification even when rendering
							//in a bulk way.
							for(i=0;i<numberOfTriangles;i++)
							{
								
								//Vertices loading
								firstOffset=dataPointer[i*numberOfArrays*3+vertexOffset];
								firstVertex=firstOffset;
								firstOffset=firstOffset*vertexStride;
								if(pVertArray[firstVertex].vPosition==NULL)
								{
									pVertArray[firstVertex].vPosition=new float*[3];
									pVertArray[firstVertex].vPosition[0]=&vertArray[firstOffset];
									pVertArray[firstVertex].vPosition[1]=&vertArray[firstOffset+1];
									pVertArray[firstVertex].vPosition[2]=&vertArray[firstOffset+2];
								}
								toBeRendered.vVertices[0+i*9]=pVertArray[firstVertex].vPosition[0];
								toBeRendered.vVertices[1+i*9]=pVertArray[firstVertex].vPosition[1];
								toBeRendered.vVertices[2+i*9]=pVertArray[firstVertex].vPosition[2];
			

								secondOffset=dataPointer[i*numberOfArrays*3+vertexOffset+numberOfArrays];
								secondVertex=secondOffset;
								secondOffset=secondOffset*vertexStride;
								if(pVertArray[secondVertex].vPosition==NULL)
								{
									pVertArray[secondVertex].vPosition=new float*[3];
									pVertArray[secondVertex].vPosition[0]=&vertArray[secondOffset];
									pVertArray[secondVertex].vPosition[1]=&vertArray[secondOffset+1];
									pVertArray[secondVertex].vPosition[2]=&vertArray[secondOffset+2];
								}
								toBeRendered.vVertices[3+i*9]=pVertArray[secondVertex].vPosition[0];
								toBeRendered.vVertices[4+i*9]=pVertArray[secondVertex].vPosition[1];
								toBeRendered.vVertices[5+i*9]=pVertArray[secondVertex].vPosition[2];
								


								thirdOffset=dataPointer[i*numberOfArrays*3+vertexOffset+numberOfArrays*2];
								thirdVertex=thirdOffset;
								thirdOffset=thirdOffset*vertexStride;
								if(pVertArray[thirdVertex].vPosition==NULL)
								{
									pVertArray[thirdVertex].vPosition=new float*[3];
									pVertArray[thirdVertex].vPosition[0]=&vertArray[thirdOffset];
									pVertArray[thirdVertex].vPosition[1]=&vertArray[thirdOffset+1];
									pVertArray[thirdVertex].vPosition[2]=&vertArray[thirdOffset+2];
								}
								toBeRendered.vVertices[6+i*9]=pVertArray[thirdVertex].vPosition[0];
								toBeRendered.vVertices[7+i*9]=pVertArray[thirdVertex].vPosition[1];
								toBeRendered.vVertices[8+i*9]=pVertArray[thirdVertex].vPosition[2];
								

								//If we have a texture, load the data for it
								if(textEnabled)
								{
									firstOffset=dataPointer[i*numberOfArrays*3+textureOffset];
									firstOffset=firstOffset*textureStride;
									if(pVertArray[firstVertex].vTextures==NULL)
									{
										pVertArray[firstVertex].vTextures=new float*[2];
										pVertArray[firstVertex].vTextures[0]=&textArray[firstOffset];
										pVertArray[firstVertex].vTextures[1]=&textArray[firstOffset+1];
									}
									toBeRendered.vTextures[0+i*6]=pVertArray[firstVertex].vTextures[0];
									toBeRendered.vTextures[1+i*6]=pVertArray[firstVertex].vTextures[1];
									


									secondOffset=dataPointer[i*numberOfArrays*3+textureOffset+numberOfArrays];
									secondOffset=secondOffset*textureStride;
									if(pVertArray[secondVertex].vTextures==NULL)
									{
										pVertArray[secondVertex].vTextures=new float*[2];
										pVertArray[secondVertex].vTextures[0]=&textArray[secondOffset];
										pVertArray[secondVertex].vTextures[1]=&textArray[secondOffset+1];
									}
									toBeRendered.vTextures[2+i*6]=pVertArray[secondVertex].vTextures[0];
									toBeRendered.vTextures[3+i*6]=pVertArray[secondVertex].vTextures[1];
									


									thirdOffset=dataPointer[i*numberOfArrays*3+textureOffset+numberOfArrays*2];
									thirdOffset=thirdOffset*textureStride;
									if(pVertArray[thirdVertex].vTextures==NULL)
									{
										pVertArray[thirdVertex].vTextures=new float*[2];
										pVertArray[thirdVertex].vTextures[0]=&textArray[thirdOffset];
										pVertArray[thirdVertex].vTextures[1]=&textArray[thirdOffset+1];
									}
									toBeRendered.vTextures[4+i*6]=pVertArray[thirdVertex].vTextures[0];
									toBeRendered.vTextures[5+i*6]=pVertArray[thirdVertex].vTextures[1];
								}

							//Normals loading
							firstOffset=dataPointer[i*numberOfArrays*3+normalOffset];
							firstOffset=firstOffset*normalStride;
							if(pVertArray[firstVertex].vNormals==NULL)
							{
								pVertArray[firstVertex].vNormals=new float*[3];
								pVertArray[firstVertex].vNormals[0]=&normArray[firstOffset];
								pVertArray[firstVertex].vNormals[1]=&normArray[firstOffset+1];
								pVertArray[firstVertex].vNormals[2]=&normArray[firstOffset+2];
							}
							toBeRendered.vNormals[0+i*9]=pVertArray[firstVertex].vNormals[0];
							toBeRendered.vNormals[1+i*9]=pVertArray[firstVertex].vNormals[1];
							toBeRendered.vNormals[2+i*9]=pVertArray[firstVertex].vNormals[2];
							


							secondOffset=dataPointer[i*numberOfArrays*3+normalOffset+numberOfArrays];
							secondOffset=secondOffset*normalStride;
							if(pVertArray[secondVertex].vNormals==NULL)
							{
								pVertArray[secondVertex].vNormals=new float*[3];
								pVertArray[secondVertex].vNormals[0]=&normArray[secondOffset];
								pVertArray[secondVertex].vNormals[1]=&normArray[secondOffset+1];
								pVertArray[secondVertex].vNormals[2]=&normArray[secondOffset+2];
							}
							toBeRendered.vNormals[3+i*9]=pVertArray[secondVertex].vNormals[0];
							toBeRendered.vNormals[4+i*9]=pVertArray[secondVertex].vNormals[1];
							toBeRendered.vNormals[5+i*9]=pVertArray[secondVertex].vNormals[2];
							



							thirdOffset=dataPointer[i*numberOfArrays*3+normalOffset+numberOfArrays*2];
							thirdOffset=thirdOffset*normalStride;
							if(pVertArray[thirdVertex].vNormals==NULL)
							{
								pVertArray[thirdVertex].vNormals=new float*[3];
								pVertArray[thirdVertex].vNormals[0]=&normArray[thirdOffset];
								pVertArray[thirdVertex].vNormals[1]=&normArray[thirdOffset+1];
								pVertArray[thirdVertex].vNormals[2]=&normArray[thirdOffset+2];
							}
							toBeRendered.vNormals[6+i*9]=pVertArray[thirdVertex].vNormals[0];
							toBeRendered.vNormals[7+i*9]=pVertArray[thirdVertex].vNormals[1];
							toBeRendered.vNormals[8+i*9]=pVertArray[thirdVertex].vNormals[2];
							
						}
						toBeRendered.iTextID=iTextureID;
						vRenderData.push_back(toBeRendered);
				}		
	}
	LoadSkeleton();
	LoadWeights();
	DrawSkeleton();
    //SetupBindPose();
	bModelLoadedCorrectly=true;
}

void ColladaModel::FindRoot(Node* nodeList)
{
	if((nodeList!=NULL)&&strcmp(nodeList->sID.c_str(),sSkeletonID.c_str()))
	{
		if(nodeList->nodes.count(sSkeletonID))
		{
			pSkeletonNode=&nodeList->nodes[sSkeletonID];
			pSkeletonNode->transformation=nodeList->transformation*pSkeletonNode->transformation;
			return;
		}
		else
		{
			std::map<core::stringc ,Node>::iterator it;
			for ( it=nodeList->nodes.begin() ; it != nodeList->nodes.end(); it++ )
			{
				FindRoot(&(*it).second);

			}
		}
	}
}

void ColladaModel::LoadSkeleton()
{
	if(pSkeletonNode!=NULL&&strcmp(sSkeletonID.c_str(),""))
	{
		JointNode root = JointNode(pSkeletonNode->sID.c_str(),pSkeletonNode->sName.c_str(),pSkeletonNode->sSID.c_str(),pSkeletonNode->sType.c_str(),pSkeletonNode->transformation);
		ColladaSkeleton=Skeleton(root);
		ColladaSkeleton.addJoint(root.getSID(),root);
		LoadJointRec(ColladaSkeleton.getJoint(root.getSID()),pSkeletonNode);
	}
}

void ColladaModel::LoadJointRec(JointNode* jParent,Node* nParent)
{
	std::map<core::stringc ,Node>::iterator it;
		for ( it=nParent->nodes.begin() ; it != nParent->nodes.end(); it++ )
		{
			JointNode current = JointNode((*it).second.sID.c_str(),(*it).second.sName.c_str(),(*it).second.sSID.c_str(),(*it).second.sType.c_str(),(*it).second.transformation);
			ColladaSkeleton.addJoint(current.getSID(),current);
			LoadJointRec(ColladaSkeleton.getJoint(current.getSID()),&it->second);
			jParent->addChild(ColladaSkeleton.getJoint(current.getSID()));
		}
}

void ColladaModel::LoadWeights()
{
	Source JointSource=skinningInformation.mSources[skinningInformation.jointSource];
	Source BindSource=skinningInformation.mSources[skinningInformation.bindSource];
	Source WeightSource=skinningInformation.mSources[skinningInformation.weightSource];
	unsigned readOffset=0;
	core::stringc currentJoint;
	//For each vertex
	for(unsigned i=0;i<skinningInformation.iWeightCount;i++)
	{
		pVertArray[i].iNJointsAffecting=skinningInformation.pVCount[i];
		pVertArray[i].pJoints=new JointNode*[pVertArray[i].iNJointsAffecting];
		//Load the influence for each vertex as defined in VCount; VCount defines the number of joints that influence this vertex
		//so you have to iterate through all of them
		for(unsigned k=0;k<pVertArray[i].iNJointsAffecting;k++)
		{
			unsigned uJointIndex=skinningInformation.pV[k*2+readOffset+skinningInformation.iJointOffset];
			//Setting the joint that influence the vertex
			pVertArray[i].pJoints[k]=ColladaSkeleton.getJointsSID(JointSource.pNameArray[uJointIndex].c_str());
			//Setting the inverse bind pose matrix for the joint
			Matrix inverse=readInvMatrix(&BindSource.pfArray,uJointIndex);
			pVertArray[i].pJoints[k]->setInverseBind(inverse);
			//Setting the proper weights for the vertex
			pVertArray[i].vWeights[k]=WeightSource.pfArray[skinningInformation.pV[k*2+readOffset+skinningInformation.iWeightOffset]];	
		}
		readOffset+=pVertArray[i].iNJointsAffecting*2;
	}
	
}

void ColladaModel::DrawSkeleton()
{
	if(strcmp(sSkeletonID.c_str(),""))
	{
		SkeletonRenderer skelRend = SkeletonRenderer();
		JointNode toBeTraversed =*ColladaSkeleton.getJoint(sSkeletonID.c_str());
		toBeTraversed.traverse_df(&skelRend);
	}
}


//189 is the first vertex to be modified
void ColladaModel::SetupBindPose()
{
	//The skinning calculation for each vertex v in a bind shape is
     //for i to n
          //v += {[(v * BSM) * IBMi * JMi] * JW}

	//For each vertex
	for(unsigned i=0;i<skinningInformation.iWeightCount;i++)
	{
		//Vertex and normals are loaded correctly
		Vector Vertex = Vector((*pVertArray[i].vPosition)[0],(*pVertArray[i].vPosition)[1],(*pVertArray[i].vPosition)[2],1);
		Vector Normal = Vector((*pVertArray[i].vNormals)[0],(*pVertArray[i].vNormals)[1],(*pVertArray[i].vNormals)[2],1);
		Vector tempVertex = Vector(0,0,0,1);
		//Vector calculated=Vector();
		Vector tempNormal = Vector();
		float TotalJointsWeight = 0;
		float NormalizedWeight = 0;
		//For each joint affecting the vertex
		for(unsigned j=0;j<pVertArray[i].iNJointsAffecting;j++)
		{
			tempVertex+=((Vertex)*(pVertArray[i].pJoints[j]->getSkinningMatrix())*pVertArray[i].vWeights[j]);
			//tempNormal    +=(((Normal*pVertArray[i].pJoints[j]->getBindShape())*pVertArray[i].pJoints[j]->getSkinningMatrix())*pVertArray[i].vWeights[j]);
			TotalJointsWeight +=pVertArray[i].vWeights[j];
		}
		if (TotalJointsWeight != 1.0f)
        {
              NormalizedWeight = 1.0f / TotalJointsWeight;
              tempVertex *= NormalizedWeight;
              tempNormal *= NormalizedWeight;
         }
		(*pVertArray[i].vPosition)[0]=tempVertex[NMS_X];
		(*pVertArray[i].vPosition)[1]=tempVertex[NMS_Y];
		(*pVertArray[i].vPosition)[2]=tempVertex[NMS_Z];
		//(*pVertArray[i].vNormals)[0]=tempNormal[NMS_X];
		//(*pVertArray[i].vNormals)[1]=tempNormal[NMS_Y];
		//(*pVertArray[i].vNormals)[2]=tempNormal[NMS_Z];
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
						return 0;
					}
				}
				break;
			}
		}
		// delete the xml parser after usage
		delete xml;
		

		//Searching for the root node in all the nodes we have loaded
		std::map<core::stringc ,Node>::iterator it;
		for ( it=mNodes.begin() ; it != mNodes.end(); it++ )
		{
			if(strcmp("",sSkeletonID.c_str()))
				FindRoot(&(*it).second);
			else
				break;
		}
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
	core::stringc sMeshName;
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
						sMeshName=xml->getAttributeValue("id");
						Mesh mesh =  Mesh();
						mesh.sID=sMeshName;
						dataRead.push_back(mesh);
						break;
					}
					else
					if (!strcmp("source",xml->getNodeName()))
					{
						Source result=readSource(xml);
						dataRead.back().sources[result.sID]=result;
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
		case EXN_ELEMENT_END:
		{
			if (!strcmp("library_geometries", xml->getNodeName()))
			{
				return;
			}
				
		}
		break;
	} 
  }
}

//Tested and working
Matrix ColladaModel::readInvMatrix(float** pFArray,unsigned boneIndex)
{
	Matrix toBeReturned=Matrix();
	for (int i=0; i<4; i++)
		for (int j=0; j<4; j++)
			toBeReturned(i+1,j+1)=(*pFArray)[i*4+j+boneIndex*16];
	return toBeReturned;
}

Matrix ColladaModel::readMatrix(IrrXMLReader* xml)
{
	Matrix toBeReturned=Matrix();
	xml->read();
	if (xml->isEmptyElement())
		return toBeReturned;
	char* charArray=(char*)xml->getNodeData();
	for (int i=1; i<=4; i++)
		for (int j=1; j<=4; j++)
			toBeReturned(i,j)=(float)strtod(charArray,&charArray);
	return toBeReturned;
}

Matrix ColladaModel::readTranslation(IrrXMLReader* xml)
{
	xml->read();//Get the data
	Matrix toBeReturned=Matrix();
	if (xml->isEmptyElement())
		return toBeReturned;
	char* charArray=(char*)xml->getNodeData();
	for (int i=1; i<=3; i++)
			toBeReturned(i,4)=(float)strtod(charArray,&charArray);
	return toBeReturned;
}

Matrix ColladaModel::readRotation(IrrXMLReader* xml)
{
	Matrix toBeReturned=Matrix();
	//Read the data now
	xml->read();
	if (xml->isEmptyElement())
			return toBeReturned;
	char* charArray=(char*)xml->getNodeData();
	Vector v=Vector();
	for (int i=1; i<=3; i++)
			v[i]=(float)strtod(charArray,&charArray);
	toBeReturned.rotV(strtod(charArray,&charArray),v);
	return toBeReturned;
}

Matrix ColladaModel::readScale(IrrXMLReader* xml)
{
	xml->read();//Get the data
	Matrix toBeReturned=Matrix();
	if (xml->isEmptyElement())
		return toBeReturned;
	char* charArray=(char*)xml->getNodeData();
	for (int i=1; i<=3; i++)
			toBeReturned(i,i)=(float)strtod(charArray,&charArray);
	return toBeReturned;
}

void ColladaModel::readNode(IrrXMLReader* xml,Node* parent)
{
	Node current = Node();
	current.sID=xml->getAttributeValue("id");
	current.sName=xml->getAttributeValue("name");
	current.sSID=xml->getAttributeValue("sid");
	current.sType=xml->getAttributeValue("type");
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("matrix", xml->getNodeName()))
				{
					Matrix debug=readMatrix(xml);
					current.transformation*=debug;
				}
				else if (!strcmp("translate", xml->getNodeName()))
				{
					current.transformation*=readTranslation(xml);
				}
				else if (!strcmp("rotate", xml->getNodeName()))
				{
					current.transformation*=readRotation(xml);
				}
				else if (!strcmp("scale", xml->getNodeName()))
				{
					current.transformation*=readScale(xml);
				}
				else if (!strcmp("node", xml->getNodeName()))
				{
					readNode(xml,&current);
				}
				else if (!strcmp("instance_controller", xml->getNodeName()))
				{
					readInstanceController(xml);
				}
			}
			break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("node", xml->getNodeName()))
				{
					
					if(parent==NULL)
					{
						//Add the node to the main node tree definition
						mNodes[current.sID]=current;
						return;
					}
					else
					{
						//Add the node to the list of nodes of its parent
						parent->nodes[current.sID]=current;
						return;
					}
				}
			}break;
		}
	}
}


void ColladaModel::readInstanceController(IrrXMLReader* xml)
{
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("skeleton", xml->getNodeName())&&!strcmp(sSkeletonID.c_str(), ""))
				{
					xml->read();
					sSkeletonID=xml->getNodeData();
					sSkeletonID.replace('#',' ');
					sSkeletonID.trim();
				}
			}
			break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("instance_controller", xml->getNodeName()))
				{
					return;
				}
			}break;
		}
	}
}


void ColladaModel::readLibraryControllers(IrrXMLReader* xml)
{
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("controller", xml->getNodeName()))
				{
					readController(xml);
				}
			}
			break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("library_controllers", xml->getNodeName()))
				{
					return;
				}
			}break;
		}
	}
}


void ColladaModel::readController(IrrXMLReader* xml)
{
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("skin", xml->getNodeName()))
				{
					readSkin(xml);
				}
			}
			break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("controller", xml->getNodeName()))
				{
					return;
				}
			}break;
		}
	}
}

void ColladaModel::readSkin(IrrXMLReader* xml)
{
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("bind_shape_matrix", xml->getNodeName()))
				{
					skinningInformation.mBindShape=readMatrix(xml);
				}
				else if (!strcmp("source", xml->getNodeName()))
				{
					Source result=readSource(xml);
					skinningInformation.mSources[result.sID]=result;
				}
				else if (!strcmp("joints", xml->getNodeName()))
				{
					readJoint(xml);
				}
				else if (!strcmp("vertex_weights", xml->getNodeName()))
				{
					readVertexWeight(xml);
				}
			}
			break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("skin", xml->getNodeName()))
				{
					return;
				}
			}break;
		}
	}
}


void ColladaModel::readJoint(IrrXMLReader* xml)
{
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if ((!strcmp("input", xml->getNodeName()))&&(!strcmp(xml->getAttributeValue("semantic"),"JOINT")))
				{
					skinningInformation.jointSource=xml->getAttributeValue("source");
					skinningInformation.jointSource.replace('#',' ');
					skinningInformation.jointSource.trim();
				}
				else if ((!strcmp("input", xml->getNodeName()))&&(!strcmp(xml->getAttributeValue("semantic"),"INV_BIND_MATRIX")))
				{
					skinningInformation.bindSource=xml->getAttributeValue("source");
					skinningInformation.bindSource.replace('#',' ');
					skinningInformation.bindSource.trim();
				}
			}
			break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("joints", xml->getNodeName()))
				{
					return;
				}
			}break;
		}
	}
}








void ColladaModel::readLibraryVisualScene(IrrXMLReader* xml)
{
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("node", xml->getNodeName()))
				{
					readNode(xml,NULL);
				}
			}
			break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("visual_scene", xml->getNodeName()))
					return;
			}break;
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
				else
				if (!strcmp("library_visual_scenes", xml->getNodeName()))
				{
					readLibraryVisualScene(xml);
				}
				else if (!strcmp("library_controllers", xml->getNodeName()))
				{
					readLibraryControllers(xml);
				}
			}break;
			case EXN_ELEMENT_END:
			{
				break; //End reading
			}
		}
	}
}

void ColladaModel::readFloatArray(IrrXMLReader* xml,float* arrayPointer)
{
	int count=xml->getAttributeValueAsInt("count");
	xml->read();
	char* charArray=(char*)xml->getNodeData();
	for (int i=0; i<count; i++)
	{
		arrayPointer[i]=strtod(charArray,&charArray);
	}
}

void ColladaModel::readVCountArray(IrrXMLReader* xml,unsigned* arrayPointer)
{
	int count=skinningInformation.iWeightCount;
	skinningInformation.iVCount=0;
	xml->read();
	char* charArray=(char*)xml->getNodeData();
	for (int i=0; i<count; i++)
	{
		arrayPointer[i]=strtod(charArray,&charArray);
		skinningInformation.iVCount+=arrayPointer[i];
	}
}

void ColladaModel::readVArray(IrrXMLReader* xml,unsigned* arrayPointer)
{
	int count=skinningInformation.iVCount*2;
	xml->read();
	char* charArray=(char*)xml->getNodeData();
	for (int i=0; i<count; i++)
	{
		arrayPointer[i]=strtod(charArray,&charArray);
	}
}

void ColladaModel::readStringArray(IrrXMLReader* xml,core::stringc* arrayPointer)
{
	int count=xml->getAttributeValueAsInt("count");
	xml->read();
	char* charArray=(char*)xml->getNodeData();
	unsigned i=0;
	unsigned h=0;
	while(i<count)
	{
		while(strncmp(charArray," ",1)&&strncmp(charArray,"",1))
		{
			arrayPointer[i].append(charArray[0]);
			charArray+=sizeof(char);
		}
		charArray+=sizeof(char);
		i++;
	}
}


Source ColladaModel::readSource(IrrXMLReader* xml)
{
	Source toBeReturned = Source();
	toBeReturned.sID=xml->getAttributeValue("id");
	toBeReturned.sName=xml->getAttributeValue("name");
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("IDREF_array", xml->getNodeName()))
				{
					toBeReturned.iIdRefArraySize=xml->getAttributeValueAsInt("count");
					toBeReturned.pIdRefArray=new core::stringc[toBeReturned.iIdRefArraySize];
					readStringArray(xml,toBeReturned.pIdRefArray);
				}
				else if (!strcmp("Name_array",xml->getNodeName()))
				{
					toBeReturned.iNameArray=xml->getAttributeValueAsInt("count");
					toBeReturned.pNameArray=new core::stringc[toBeReturned.iNameArray];
					readStringArray(xml,toBeReturned.pNameArray);
				}
				else if (!strcmp("float_array", xml->getNodeName()))
				{
					toBeReturned.iFArraySize=xml->getAttributeValueAsInt("count");
					toBeReturned.pfArray=new float[toBeReturned.iFArraySize];
					readFloatArray(xml,toBeReturned.pfArray);
				}
				else if (!strcmp("accessor", xml->getNodeName()))
				{
					toBeReturned.count  = xml->getAttributeValueAsInt("count");
					toBeReturned.offset = xml->getAttributeValueAsInt("offset");
					toBeReturned.stride = xml->getAttributeValueAsInt("stride");
						if (toBeReturned.stride == 0)
							toBeReturned.stride = 1;   //Fix a bug in some models
					xml->read(); //Skip to the parameter section
					toBeReturned.sParameterType=xml->getAttributeValue("name");
				}
			}
			break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("source", xml->getNodeName()))
				{
					return toBeReturned;
				}
			}break;
		}
	}
}


void ColladaModel::readVertexWeight(IrrXMLReader* xml)
{
	skinningInformation.iWeightCount=xml->getAttributeValueAsInt("count");
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if ((!strcmp("input", xml->getNodeName()))&&(!strcmp(xml->getAttributeValue("semantic"),"JOINT")))
				{
					skinningInformation.iJointOffset=xml->getAttributeValueAsInt("offset");
				}
				else if ((!strcmp("input", xml->getNodeName()))&&(!strcmp(xml->getAttributeValue("semantic"),"WEIGHT")))
				{
					skinningInformation.iWeightOffset=xml->getAttributeValueAsInt("offset");
					skinningInformation.weightSource=xml->getAttributeValue("source");
					skinningInformation.weightSource.replace('#',' ');
					skinningInformation.weightSource.trim();
				}
				else if (!strcmp("vcount", xml->getNodeName()))
				{
					skinningInformation.pVCount=new unsigned[skinningInformation.iWeightCount];
					readVCountArray(xml,skinningInformation.pVCount);
				}
				else if (!strcmp("v", xml->getNodeName()))
				{
					skinningInformation.pV=new unsigned[skinningInformation.iVCount*2];
					readVArray(xml,skinningInformation.pV);
				}
			}break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("vertex_weights", xml->getNodeName()))
				{
					return;
				}
			}break;
		}
	}
}
