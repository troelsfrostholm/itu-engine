#include "LinearMath/btAlignedObjectArray.h"

class btBroadphaseInterface;
class btCollisionShape;
class btOverlappingPairCache;
class btCollisionDispatcher;
class btConstraintSolver;
struct btCollisionAlgorithmCreateFunc;
class btDefaultCollisionConfiguration;
class	btCollisionShape;
class	btDynamicsWorld;
class	btRigidBody;

class nms_physics
{
	btDynamicsWorld* dynamicsWorld;

	//keep the collision shapes, for deletion/cleanup
	btAlignedObjectArray<btCollisionShape*>	collisionShapes;

	btBroadphaseInterface*	broadphase;

	btCollisionDispatcher*	dispatcher;

	btConstraintSolver*	solver;

	btDefaultCollisionConfiguration* collisionConfiguration;

	public:
		nms_physics(){}
		virtual ~nms_physics()
		{
			exitPhysics();
		}
		void initPhysics();
		void exitPhysics();
		void simulatePhysics();

		btDynamicsWorld* getDynamicsWorld()
		{
			return dynamicsWorld;
		}

		void addRBody(btRigidBody* body);

		static nms_physics* create()
		{
			nms_physics* physics = new nms_physics;
			physics->initPhysics();
			return physics;
		}	
};

class NMS_KinematicMotionState : public btMotionState {
public:
    NMS_KinematicMotionState(const btTransform &initialpos) {
        Pos = initialpos;
    }

    virtual ~ MyKinematicMotionState() {
    }

    virtual void getWorldTransform(btTransform &worldTrans) const {
        worldTrans = Pos;
    }

    void setKinematicPos(btTransform &currentPos) {
        Pos = currentPos;
    }

    virtual void setWorldTransform(const btTransform &worldTrans) {
    }

protected:
    btTransform Pos;
};

