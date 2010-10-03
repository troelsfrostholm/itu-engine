#include "NMS_LightSystem.h"



/***************LIGHTSOURCE CLASS DEFINITION**************************************/
LightSource::LightSource()
{
	eLightNumber=NULL;                //Number of the light to be generated
	vLightValue=Vector();				//Value of the light
	fIntDis=NULL;
	eAttType=NULL;
	fAttFactor=NULL;
	bToBeUpdated=false;
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

void  LightSource::setLightNumber(GLenum eNumber)
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
	glEnable(bShading);
	glShadeModel(GL_SMOOTH);
}

void NMS_LightSystem::Disable()
{
	glDisable(GL_LIGHTING);
}

AmbientLight::AmbientLight()
{
	bToBeUpdated=false;
}

void AmbientLight::setGlobalAmbient(const Vector* vVector)
{
	bToBeUpdated=true;
	aLightValue[0]=(*vVector)[1];
	aLightValue[1]=(*vVector)[2];
	aLightValue[2]=(*vVector)[3];
	aLightValue[3]=(*vVector)[4];
}

Vector AmbientLight::getGlobalAmbient()
{
	Vector temp=Vector();
	temp[1]=aLightValue[0];
	temp[2]=aLightValue[1];
	temp[3]=aLightValue[2];
	temp[4]=aLightValue[3];
	return temp;
}

void AmbientLight::render(float time)
{
	if(bToBeUpdated)
	{
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, aLightValue);
		bToBeUpdated=false;
	}
}

void LightSource::defineLight(LightSource source)
{
	if(source.getLightNumber()==NULL)
		throw 0;
	else
	{
			(*this)=source;
			bToBeUpdated=true;
	}
	
}

void LightSource::render(float time)
{
	if(bToBeUpdated)
	{
		    GLfloat* temp=new GLfloat[4];
			temp[0]=(getPosVector())[1];
			temp[1]=(getPosVector())[2];
			temp[2]=(getPosVector())[3];
			temp[3]=(getPosVector())[4];
			glLightfv(getLightNumber(),GL_POSITION,temp);
			temp[0]=(getLightValue())[1];
			temp[1]=(getLightValue())[2];
			temp[2]=(getLightValue())[3];
			temp[3]=(getLightValue())[4];
			glLightfv(getLightNumber(),GL_AMBIENT_AND_DIFFUSE,temp);
			temp=new GLfloat[3];
			temp[0]=(getDirVector())[1];
			temp[1]=(getDirVector())[2];
			temp[2]=(getDirVector())[3];
			glLightfv(getLightNumber(),GL_SPOT_DIRECTION,temp);
			glEnable(getLightNumber());
			bToBeUpdated=false;
	}
}