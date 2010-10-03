#include "nms_physics.h"
#include <iostream>

void nms_physics::initPhysics()
{
	broadphase = new btDbvtBroadphase();

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-10,0));
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

	delete dynamicsWorld;
	
	delete solver;
	
	delete broadphase;
	
	delete dispatcher;

	delete collisionConfiguration;
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
	dt = dt /1000000.f; //Convert microseconds to seconds
	clock.reset();
	return dt;
}