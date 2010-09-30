#include "NMS_3DObject.h"


NMS_3DObject::NMS_3DObject()
{
	vPos=Vector();
	vDir=Vector();
}


NMS_3DObject::~NMS_3DObject()
{}


NMS_3DObject::NMS_3DObject(const Vector* vPos,const Vector* vDir)
{
	this->vPos=(*vPos);
	this->vDir=(*vDir);
}

Vector NMS_3DObject::getPosVector()
{
	return this->vPos;
}

void   NMS_3DObject::setPosVector(const Vector* vPos)
{
	this->vPos=(*vPos);
}

Vector NMS_3DObject::getDirVector()
{
	return this->vDir;
}

void   NMS_3DObject::setDirVector(const Vector* vDir)
{
	this->vDir=(*vDir);
}


ostream& operator<<(ostream& output, const NMS_3DObject& obj)
{
	output<<"Position:";
	output<<obj.vPos;
	output<<'\n';
	output<<"Direction:";
	output<<obj.vDir;
	output<<'\n';
	return output;
};

bool operator == (const NMS_3DObject &a, const NMS_3DObject &b)
{
	if((a.vDir!=b.vDir)||(a.vPos!=b.vPos))
		return false;
	return true;
}