#ifdef __EXP_NMS_PHYSICS
#    define PHYSICS_D __declspec(dllexport)
#else
#    define PHYSICS_D __declspec(dllimport)
#endif

#ifndef __NMS_PHYSICS
#define __NMS_PHYSICS

#include "LinearMath/btAlignedObjectArray.h"
#include <btBulletDynamicsCommon.h>
#include "BulletCollision\CollisionDispatch\btGhostObject.h"
#include <vector>
#include <boost/function.hpp>

#pragma warning( disable: 4251 )  //Used to disable this useless warning: http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html

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
class btGhostObject;

class PHYSICS_D nms_physics
{
	private:
		btDynamicsWorld* dynamicsWorld;
		//Keep collision shapes, for deletion/cleanup
		btAlignedObjectArray<btCollisionShape*>	collisionShapes;
		btBroadphaseInterface*	broadphase;
		btCollisionDispatcher*	dispatcher;
		btConstraintSolver*	solver;
		btDefaultCollisionConfiguration* collisionConfiguration;
		btGhostPairCallback *m_ghostPairCallback;
		btClock clock;
		std::vector<btPairCachingGhostObject*> triggers;
		boost::function1<void, int> triggerCallback;

	public:
		nms_physics();
		~nms_physics();
		void initPhysics();
		void exitPhysics();
		void simulatePhysics();
		void addRBody(btRigidBody* body);
		btScalar getDeltaTimeSeconds();
		void createTrigger(btCollisionShape *triggershape, btTransform &position, void (_callback)(int i));
		int checkTrigger(btPairCachingGhostObject *ghostObject);
		void checkAllTriggers();
		btDynamicsWorld* getDynamicsWorld();
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