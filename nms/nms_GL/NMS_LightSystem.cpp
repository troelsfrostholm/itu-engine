#include "NMS_LightSystem.h"



/***************LIGHTSOURCE CLASS DEFINITION**************************************/
LightSource::LightSource()
{
	eLightNumber=NULL;                //Number of the light to be generated
	vLightValue=Vector();				//Value of the light
	fIntDis=NULL;
	eAttType=NULL;
	fAttFactor=NULL;
}

GLfloat LightSource::getAttFact()
{return fAttFactor;}

GLenum LightSource::getAttType()
{return eAttType;}

GLfloat LightSource::getIntDis()
{return fIntDis;}

GLenum LightSource::getLightNumber()
{return eLightNumber;}

Vector LightSource::getLightValue()
{return vLightValue;}

void       LightSource::setLightNumber(GLenum eNumber)
{eLightNumber=eNumber;}

void LightSource::setLightValue(const Vector* vValue)
{
	vLightValue=(*vValue);
}

void       LightSource::setIntDis(GLfloat fIntDist)
{fIntDis=fIntDist;}
void       LightSource::setAttType(GLenum eType)
{eAttType=eType;}
void       LightSource::setAttFact(GLfloat fAttFact)
{fAttFactor=fAttFact;}


/************************************************************************/



/***************LIGHTSYSTEM CLASS DEFINITION**************************************/

NMS_LightSystem::NMS_LightSystem()
{}

void NMS_LightSystem::Enable(int bShading)
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	//glEnable(bShading);
	glShadeModel(GL_SMOOTH);
}

void NMS_LightSystem::Disable()
{
	glDisable(GL_LIGHTING);
}

void NMS_LightSystem::setGlobalAmbient(const Vector* vVector)
{
	GLfloat* temp=new GLfloat[4];
			temp[0]=(*vVector)[1];
			temp[1]=(*vVector)[2];
			temp[2]=(*vVector)[3];
			temp[3]=(*vVector)[4];
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, temp);
}

void NMS_LightSystem::defineLight(LightSource source)
{
	if(source.getLightNumber()==NULL)
		throw 0;
	else
	{
			GLfloat* temp=new GLfloat[4];
			temp[0]=(source.getPosVector())[1];
			temp[1]=(source.getPosVector())[2];
			temp[2]=(source.getPosVector())[3];
			temp[3]=(source.getPosVector())[4];
			glLightfv(source.getLightNumber(),GL_POSITION,temp);
			temp[0]=(source.getLightValue())[1];
			temp[1]=(source.getLightValue())[2];
			temp[2]=(source.getLightValue())[3];
			temp[3]=(source.getLightValue())[4];
			glLightfv(source.getLightNumber(),GL_AMBIENT_AND_DIFFUSE,temp);
			temp=new GLfloat[3];
			temp[0]=(source.getDirVector())[1];
			temp[1]=(source.getDirVector())[2];
			temp[2]=(source.getDirVector())[3];
			glLightfv(source.getLightNumber(),GL_SPOT_DIRECTION,temp);
	}
	glEnable(source.getLightNumber());
}