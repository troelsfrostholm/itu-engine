#include "nms_physics.h"
#include <iostream>

void nms_physics::initPhysics()
{
	broadphase = new(STATIC_ALLOC, MEM_PERSISTENT) btDbvtBroadphase();

    collisionConfiguration = new(STATIC_ALLOC, MEM_PERSISTENT) btDefaultCollisionConfiguration();
    dispatcher = new(STATIC_ALLOC, MEM_PERSISTENT) btCollisionDispatcher(collisionConfiguration);

    solver = new(STATIC_ALLOC, MEM_PERSISTENT) btSequentialImpulseConstraintSolver;

    dynamicsWorld = new(STATIC_ALLOC, MEM_PERSISTENT) btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-10,0));

	m_ghostPairCallback = new(STATIC_ALLOC, MEM_PERSISTENT) btGhostPairCallback();
	dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostPairCallback);

	triggers = std::vector<btPairCachingGhostObject*>();

	debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld->setDebugDrawer(&debugDrawer);

}

void nms_physics::exitPhysics()
{
	int i;
	for (i=dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<collisionShapes.size();j++)
	{
		btCollisionShape* shape = collisionShapes[j];
		delete shape;
	}

	//delete dynamicsWorld;
	
	//delete solver;
	
	//delete broadphase;
	
	//delete dispatcher;

	//delete collisionConfiguration;
}

void nms_physics::simulatePhysics()
{
	float s = getDeltaTimeSeconds();
	dynamicsWorld->stepSimulation(s, 1);
}

void nms_physics::addRBody(btRigidBody* body)
{
	dynamicsWorld->addRigidBody(body);
}

btDynamicsWorld* nms_physics::getDynamicsWorld()
{
	return dynamicsWorld;
}

nms_physics::nms_physics()
{
	initPhysics();
}

nms_physics::~nms_physics()
{
	exitPhysics();
}

/* This code is taken from the Bullet demo */
btScalar nms_physics::getDeltaTimeSeconds()
{
	btScalar dt = (btScalar)clock.getTimeMicroseconds();
	dt = dt / 1000000.f; //Convert microseconds to seconds
	clock.reset();
	return dt;
}

void nms_physics::createTrigger(btCollisionShape *triggershape, btTransform &position, void (_callback)(int i))
{
	btPairCachingGhostObject *TriggerGhostObject = new btPairCachingGhostObject();
	TriggerGhostObject->setCollisionShape(triggershape);
	TriggerGhostObject->setWorldTransform(position);

	TriggerGhostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);

	dynamicsWorld->addCollisionObject(TriggerGhostObject);

	triggers.push_back(TriggerGhostObject);

	triggerCallback = _callback;
}

void nms_physics::checkAllTriggers()
{
	for(unsigned int i=0; i < triggers.size(); i++) 
	{
		checkTrigger(triggers[i]);
	}
}

int nms_physics::checkTrigger(btPairCachingGhostObject *ghostObject)
{
	int triggered = 0;
	btAlignedObjectArray<btCollisionObject*>& overlappingObjects = ghostObject->getOverlappingPairs();	
	const int numObjects = overlappingObjects.size();	
	for(int i=0; i<numObjects; i++)
	{
		btCollisionObject*	colObject = overlappingObjects[i];
		/// Do something with colObj
		triggered = 1;
		triggerCallback(triggered);
		break;
	}
	return triggered;
}