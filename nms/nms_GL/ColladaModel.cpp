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
	sSkeletonID				="";
	pSkeletonNode           =NULL;
	iCurrentFrame			=0;
	fAnimationTime			=0.05f;
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

	 vVertices=NULL;
	 vTextures=NULL;;
	 vNormals=NULL;;

	 vertexStride=0;
	 textureStride=0;
	 normalStride=0;
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
				DrawSkeleton(time);
				SetupBindPose();
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
		for(unsigned k=0;k<vRenderData[i].iTriangleCount;k++)
		{
			glBegin(GL_TRIANGLES);
				if(vRenderData[i].textureStride>2)
					glTexCoord3fv(&textArray[vRenderData[i].vTextures[k*vRenderData[i].textureStride]*vRenderData[i].textureStride]);
				else
					glTexCoord2fv(&textArray[vRenderData[i].vTextures[k*vRenderData[i].textureStride]*vRenderData[i].textureStride]);

				glVertex3fv(&vertArray[vRenderData[i].vVertices[k*vRenderData[i].vertexStride]*vRenderData[i].vertexStride]);

				
				if(vRenderData[i].textureStride>2)
					glTexCoord3fv(&textArray[vRenderData[i].vTextures[1+k*vRenderData[i].textureStride]*vRenderData[i].textureStride]);
				else
					glTexCoord2fv(&textArray[vRenderData[i].vTextures[1+k*vRenderData[i].textureStride]*vRenderData[i].textureStride]);
				glVertex3fv(&vertArray[vRenderData[i].vVertices[1+k*vRenderData[i].vertexStride]*vRenderData[i].vertexStride]);
				
				if(vRenderData[i].textureStride>2)
					glTexCoord3fv(&textArray[vRenderData[i].vTextures[2+k*vRenderData[i].textureStride]*vRenderData[i].textureStride]);
				else
					glTexCoord2fv(&textArray[vRenderData[i].vTextures[2+k*vRenderData[i].textureStride]*vRenderData[i].textureStride]);
				glVertex3fv(&vertArray[vRenderData[i].vVertices[2+k*vRenderData[i].vertexStride]*vRenderData[i].vertexStride]);
			glEnd();
		}
	}
	
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

for(m=0;m<maxVertices;m++)
{
	pVertArray[m].vNormals=NULL;
	pVertArray[m].vPosition=NULL;
	pVertArray[m].vTextures=NULL;
}

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

							toBeRendered.vertexStride=vertexStride;
							toBeRendered.textureStride=textureStride;
							toBeRendered.normalStride=normalStride;

							int* dataPointer=dataRead[m].triangles[t].pTriangleData;
							toBeRendered.dataPointer=dataPointer;
							vertArray=positionSource->pfArray;
							copiedPositions=new float[positionSource->count*3];
							for(int l=0;l<positionSource->count*3;l++)
							{
								copiedPositions[l]=vertArray[l];
							}
							textArray=textureSource->pfArray;
							normArray=normalSource->pfArray;

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

							toBeRendered.vTextures=new(LEVEL_ALLOC, MEM_LEVEL) unsigned[numberOfTriangles*textureStride];
							toBeRendered.vNormals=new(LEVEL_ALLOC, MEM_LEVEL)  unsigned[numberOfTriangles*3*normalStride];
							toBeRendered.vVertices=new(LEVEL_ALLOC, MEM_LEVEL) unsigned[numberOfTriangles*vertexStride];



							
							for(i=0;i<numberOfTriangles;i++)
							{
								
								//Load the index of the vertices to be rendered
								firstOffset=dataPointer[i*numberOfArrays*3+vertexOffset];
								toBeRendered.vVertices[0+i*vertexStride]=firstOffset;
								
								secondOffset=dataPointer[i*numberOfArrays*3+vertexOffset+numberOfArrays];
								toBeRendered.vVertices[1+i*vertexStride]=secondOffset;
								
								thirdOffset=dataPointer[i*numberOfArrays*3+vertexOffset+numberOfArrays*2];
								toBeRendered.vVertices[2+i*vertexStride]=thirdOffset;
								

								//If we have a texture, load the data for it
								if(textEnabled)
								{
									firstOffset=dataPointer[i*numberOfArrays*3+textureOffset];
									toBeRendered.vTextures[0+i*textureStride]=firstOffset;
									
									secondOffset=dataPointer[i*numberOfArrays*3+textureOffset+numberOfArrays];
									toBeRendered.vTextures[1+i*textureStride]=secondOffset;
									
									if(textureStride>2)
									{
										thirdOffset=dataPointer[i*numberOfArrays*3+textureOffset+numberOfArrays*2];
										toBeRendered.vTextures[2+i*textureStride]=thirdOffset;
									}
								}

							//Normals loading
							firstOffset=dataPointer[i*numberOfArrays*3+normalOffset];
							firstOffset=firstOffset*normalStride;
							
							toBeRendered.vNormals[0+i*9]=firstOffset;
							toBeRendered.vNormals[1+i*9]=firstOffset+1;
							toBeRendered.vNormals[2+i*9]=firstOffset+2;
							


							secondOffset=dataPointer[i*numberOfArrays*3+normalOffset+numberOfArrays];
							secondOffset=secondOffset*normalStride;
							
							toBeRendered.vNormals[3+i*9]=secondOffset;
							toBeRendered.vNormals[4+i*9]=secondOffset+1;
							toBeRendered.vNormals[5+i*9]=secondOffset+2;
							



							thirdOffset=dataPointer[i*numberOfArrays*3+normalOffset+numberOfArrays*2];
							thirdOffset=thirdOffset*normalStride;
							
							toBeRendered.vNormals[6+i*9]=thirdOffset;
							toBeRendered.vNormals[7+i*9]=thirdOffset+1;
							toBeRendered.vNormals[8+i*9]=thirdOffset+2;
							
						}
						toBeRendered.iTextID=iTextureID;
						vRenderData.push_back(toBeRendered);
				}		
	}
	LoadSkeleton();
	LoadWeights();
	LoadAnimationData();
	bModelLoadedCorrectly=true;
}

void ColladaModel::FindRoot(Node* nodeList)
{
	if((nodeList!=NULL)&&strcmp(nodeList->sID.c_str(),sSkeletonID.c_str()))
	{
		if(nodeList->nodes.count(sSkeletonID))
		{
			pSkeletonNode=&nodeList->nodes[sSkeletonID];
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
	//It's already our root
	else if((nodeList!=NULL)&&!strcmp(nodeList->sID.c_str(),sSkeletonID.c_str()))
	{
		pSkeletonNode=nodeList;
		return;
	}
}



//CHECK THE ORDER OF LOADING THE MATRIX, IT COULD LEAD TO PROBLEMS!
//Load animation data for the current Joint
void ColladaModel::LoadAnimationData()
{
	//Iterate through all the animations to get the one we are interested into
	for (int i=0; i<vAnimation.size(); i++) 
	{
			//Read the ID of the animation and remove the "-transform" subfix to get the target joint we should apply the animation to
			string sTargetJoint=(vAnimation[i].sID.subString(0,vAnimation[i].sID.size()-10)).c_str();

			//Retrieve the targeted node from the skeleton
			JointNode *target=ColladaSkeleton.getJoint(sTargetJoint);
			//Create enough space to save the keyframes for the current joint
			target->initializeKeyframes(vAnimation[i].vSources[0].count);
			//Reconstruct the animation matrixes for each channels
			for(int j=0;j<target->getNKeyFrames();j++)
			{
				KeyFrame currentFrame=KeyFrame();
				//Set the time for the current frame. Each frame source follows the same
				//time structure by assumption so we can just take the time from the first source
				currentFrame.setTime(vAnimation[i].vSources[0].pfArray[j]);

				//Load the matrix for the current frame now
				Matrix frameMatrix=Matrix();
				for(int s=0;s<16;s++)
				{
					unsigned row = (int) (s / 4)+1;
					unsigned col = (int) (s % 4)+1;
					//Skip bugged animation
					if(!(vAnimation[i].vSources.size()<16*3))
					{
						//We have less values, that means the values are constants through the period of time, just take the first one
						if(frameMatrix(row,col)=vAnimation[i].vSources[s*3+1].count<(target->getNKeyFrames()))
							frameMatrix(row,col)=vAnimation[i].vSources[s*3+1].pfArray[0];
						else
							frameMatrix(row,col)=vAnimation[i].vSources[s*3+1].pfArray[j];
						//Fix a bug, the last element of the matrix should be 1, just be sure it is!
						if(s==15)
						{
							frameMatrix(row,col)=1;
						}
					}
				}
				currentFrame.setTransform(&frameMatrix);
				target->setKeyFrame(currentFrame,j);
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

void ColladaModel::DrawSkeleton(float time)
{
	if(strcmp(sSkeletonID.c_str(),""))
	{
		SkeletonRenderer skelRend = SkeletonRenderer();
		skelRend.setAnimationTime(time);
		JointNode toBeTraversed =*ColladaSkeleton.getJoint(sSkeletonID.c_str());
		toBeTraversed.traverse_df(&skelRend);
	}
}

void ColladaModel::SetupBindPose()
{
	//The skinning calculation for each vertex v in a bind shape is
     //for i to n
          //v += {[(v * BSM) * IBMi * JMi] * JW}
	int i=0;
	//For each vertex
	for(unsigned i=0;i<skinningInformation.iWeightCount;i++)
	{
		//Restore the original values
		vertArray[i*3]=copiedPositions[i*3];
		vertArray[i*3+1]=copiedPositions[i*3+1];
		vertArray[i*3+2]=copiedPositions[i*3+2];
		//Vertex and normals are loaded correctly
		Vector Vertex = Vector(vertArray[i*3],vertArray[i*3+1],vertArray[i*3+2],1);
		//Vector Normal = Vector((*pVertArray[i].vNormals)[0],(*pVertArray[i].vNormals)[1],(*pVertArray[i].vNormals)[2],1);
		Vector tempVertex = Vector();
		Vector tempNormal = Vector();
		float TotalJointsWeight = 0;
		float NormalizedWeight = 0;
		//For each joint affecting the vertex
		for(unsigned j=0;j<pVertArray[i].iNJointsAffecting;j++)
		{
			tempVertex+=((Vertex*skinningInformation.mBindShape)*(pVertArray[i].pJoints[j]->getSkinningMatrix())*pVertArray[i].vWeights[j]);
			//tempNormal+=((Normal*skinningInformation.mBindShape)*(pVertArray[i].pJoints[j]->getSkinningMatrix())*pVertArray[i].vWeights[j]);
			TotalJointsWeight +=pVertArray[i].vWeights[j];
		}
		if (TotalJointsWeight != 1.0f)
        {
              NormalizedWeight = 1.0f / TotalJointsWeight;
              tempVertex *= NormalizedWeight;
              tempNormal *= NormalizedWeight;
         }
		vertArray[i*3]=tempVertex[NMS_X];
		vertArray[i*3+1]=tempVertex[NMS_Y];
		vertArray[i*3+2]=tempVertex[NMS_Z];
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
									}
								}break;
							case EXN_ELEMENT_END:
								{
									if (!strcmp("profile_COMMON", xml->getNodeName()))
									{
										effectEnded=true;
										vEffects.push_back(cEffect);
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
							}
							else
							if(!strcmp("input",xml->getNodeName()))
							{
								triangle.uNumberOfData++;
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

Input ColladaModel::readInput(IrrXMLReader* xml)
{
	Input toBeReturned=Input();
	toBeReturned.sSemantic=xml->getAttributeValue("semantic");
	toBeReturned.sSource=xml->getAttributeValue("source");
	return toBeReturned;
}

Sample ColladaModel::readSample(IrrXMLReader* xml)
{
	Sample toBeReturned=Sample();
	toBeReturned.sID=xml->getAttributeValue("id");
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("input", xml->getNodeName()))
				{
					Input input=readInput(xml);
					toBeReturned.mInput[input.sSource]=input;
				}
			}
			break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("sampler", xml->getNodeName()))
				{
					return toBeReturned;
				}
			}break;
		}
	}
}

Channel ColladaModel::readChannel(IrrXMLReader* xml)
{
	Channel toBeReturned=Channel();
	toBeReturned.sSource=xml->getAttributeValue("source");
	toBeReturned.sTarget=xml->getAttributeValue("target");
	return toBeReturned;
}



void ColladaModel::readLibraryAnimations(IrrXMLReader* xml)
{
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("animation", xml->getNodeName()))
				{
					readAnimation(xml);
				}
			}
			break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("library_animations", xml->getNodeName()))
				{
					return;
				}
			}break;
		}
	}
}

void ColladaModel::readAnimation(IrrXMLReader* xml)
{
	Animation toBeReturned=Animation();
	toBeReturned.sID=xml->getAttributeValue("id");
	while(xml->read())
	{
		switch(xml->getNodeType())
		{
			case EXN_ELEMENT:
			{
				if (!strcmp("source", xml->getNodeName()))
				{
					toBeReturned.vSources.push_back(readSource(xml));
				}
				else
				if (!strcmp("sampler", xml->getNodeName()))
				{
					toBeReturned.vSamples.push_back(readSample(xml));
				}
				else
				if (!strcmp("channel", xml->getNodeName()))
				{
					toBeReturned.vChannels.push_back(readChannel(xml));
				}
			}
			break;
			case EXN_ELEMENT_END:
			{
				if (!strcmp("animation", xml->getNodeName()))
				{
					vAnimation.push_back(toBeReturned);
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
				if (!strcmp("library_animations", xml->getNodeName()))
				{
					//Read animations
					readLibraryAnimations(xml);
				}
				else
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
					toBeReturned.sType=xml->getAttributeValue("type");
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
