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
#include "nms_debugDrawer.h"

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

		btDynamicsWorld *dynamicsWorld;

		//Keep collision shapes, for deletion/cleanup
		btAlignedObjectArray<btCollisionShape*>	collisionShapes;
		
		btBroadphaseInterface *broadphase;
		btCollisionDispatcher*	dispatcher;
		btConstraintSolver*	solver;
		btDefaultCollisionConfiguration* collisionConfiguration;

		btGhostPairCallback *m_ghostPairCallback;

		btClock clock;

		//triggers
		std::vector<btPairCachingGhostObject*> triggers;
		boost::function1<void, int> triggerCallback;

		//debug drawer
		nms_debugDrawer debugDrawer;

	public:
		nms_physics();
		~nms_physics();
		void initPhysics();
		void exitPhysics();
		void simulatePhysics();
		void addRBody(btRigidBody* body);
		void setDebugMode(bool state);
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
			userSetPosition = false;
		}

		virtual ~NMS_KinematicMotionState() {
		}

		virtual void getWorldTransform(btTransform &worldTrans) const
		{
			worldTrans = position;
		}

		void setKinematicPos(btTransform &currentPos)
		{
			userPosition = currentPos;
			userSetPosition = true;
		}

		virtual void setWorldTransform(const btTransform &worldTrans){
			position = worldTrans;
			if(userSetPosition) {
				position = userPosition;
				userSetPosition = false;
			}
		}

	protected:
		btTransform position;
		btTransform userPosition;
		bool userSetPosition;
};

class PHYSICS_D NMS_RigidBody
{

};

#endif