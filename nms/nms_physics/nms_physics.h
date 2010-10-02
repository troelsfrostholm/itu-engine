#ifdef __EXP_NMS_PHYSICS
#    define PHYSICS_D __declspec(dllexport)
#else
#    define PHYSICS_D __declspec(dllimport)
#endif

#ifndef __NMS_PHYSICS
#define __NMS_PHYSICS

#include "LinearMath/btAlignedObjectArray.h"
#include <btBulletDynamicsCommon.h>

class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;
class btCollisionShape;
class btDynamicsWorld;
class btRigidBody;

class PHYSICS_D nms_physics
{
	btDynamicsWorld* dynamicsWorld;

	//Keep collision shapes, for deletion/cleanup
	btAlignedObjectArray<btCollisionShape*>	collisionShapes;

	btBroadphaseInterface*	broadphase;

	btCollisionDispatcher*	dispatcher;

	btConstraintSolver*	solver;

	btDefaultCollisionConfiguration* collisionConfiguration;

	public:
		nms_physics();
		~nms_physics();
		void initPhysics();
		void exitPhysics();
		void simulatePhysics(float ms);

		btDynamicsWorld* getDynamicsWorld()
		{
			return dynamicsWorld;
		}

		void addRBody(btRigidBody* body);
};

class PHYSICS_D NMS_KinematicMotionState : public btMotionState 
{
	public:
		NMS_KinematicMotionState(const btTransform &initialpos)
		{
			position = initialpos;
		}

		virtual ~NMS_KinematicMotionState() {
		}

		virtual void getWorldTransform(btTransform &worldTrans) const
		{
			worldTrans = position;
		}

		void setKinematicPos(btTransform &currentPos)
		{
			position = currentPos;
		}

		virtual void setWorldTransform(const btTransform &worldTrans){
		}

	protected:
		btTransform position;
};
#endif