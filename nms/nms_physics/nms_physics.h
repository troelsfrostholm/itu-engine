#ifdef __EXP_NMS_PHYSICS
#    define PHYSICS_D __declspec(dllexport)
#else
#    define PHYSICS_D __declspec(dllimport)
#endif

#ifndef __NMS_PHYSICS
#define __NMS_PHYSICS

#include "NxPhysics.h"
#include <boost/function.hpp>
#include "nms_debugDrawer.h"

class PHYSICS_D nms_physics
{
	private:

		btDynamicsWorld* dynamicsWorld;

		
		btBroadphaseInterface*	broadphase;



		//triggers

		//debug drawer
		nms_debugDrawer debugDrawer;

	public:
		nms_physics();
		~nms_physics();
		void InitNx();
		NxScene* getScene();
		void addActor(NxActor *a);
		void exitPhysics();
		void StartPhysics();
		void GetPhysicsResults();
		void simulatePhysics();
};


class PHYSICS_D NMS_RigidBody
{

};

#endif