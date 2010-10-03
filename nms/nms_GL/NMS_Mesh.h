#ifdef __EXP_NMS_GL
#    define NMSMESH_D __declspec(dllexport)
#else
#    define NMSMESH_D __declspec(dllimport)
#endif

#ifndef __NMS_MESH_H__
#define __NMS_MESH_H__

class NMSMESH_D NMS_Mesh
{
	public:
		NMS_Mesh(){};
		~NMS_Mesh(){};
		virtual void render(float time) = 0;
};

#endif