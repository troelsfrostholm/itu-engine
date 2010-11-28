#include "NMS_Mesh.h"

NMS_Mesh::NMS_Mesh()
{
	material.ambient = Vector(1, 1, 1, 1);
	material.diffuse = Vector(0, 0, 0, 0);
	material.specular = Vector(0, 0, 0, 0);
	material.shininess = 1;
	material.sphereMap = "";
	material.smapTexId = 0;
}

void NMS_Mesh::setMaterialGL()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE);
	float ambient[4];
	float diffuse[4];
	float specular[4];

	material.ambient.floatArray(ambient);
	material.diffuse.floatArray(diffuse);
	material.specular.floatArray(specular);

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &material.shininess);

	if(material.sphereMap != "") {
		if(material.smapTexId == 0) {
			char * tmp = new(LEVEL_ALLOC, MEM_LEVEL) char[material.sphereMap.length()+1];
			strcpy(tmp, material.sphereMap.c_str());
			material.smapTexId = NMS_ASSETMANAGER.LoadTexture(material.sphereMap.c_str(), tmp);
		}
		glBindTexture(GL_TEXTURE_2D, material.smapTexId);
		NMS_SHADER_MANAGER->setShaderAttribute("environmentmap", 1);
	}
	else {
		NMS_SHADER_MANAGER->setShaderAttribute("environmentmap", 0);
	}
}