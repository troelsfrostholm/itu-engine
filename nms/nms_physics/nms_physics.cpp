// nms_physics.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "nms_physics.h"
#include <iostream>
#include <btBulletDynamicsCommon.h>

void nms_physics::initPhysics(){

	broadphase = new btDbvtBroadphase();

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-10,0));
}

void nms_physics::exitPhysics(){

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

void nms_physics::simulatePhysics(){
	dynamicsWorld->stepSimulation(1/60.f,10);
}

void nms_physics::addRBody(btRigidBody* body){
	dynamicsWorld->addRigidBody(body);
}

int main (void)
{
	nms_physics* physics = nms_physics::create();
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

    btCollisionShape* fallShape = new btSphereShape(1);


    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo
            groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	physics->addRBody(groundRigidBody);


    btDefaultMotionState* fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
    btScalar mass = 1;
    btVector3 fallInertia(0,0,0);
    fallShape->calculateLocalInertia(mass,fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
    physics->addRBody(fallRigidBody);


    for (int i=0 ; i<300 ; i++) {
            physics->simulatePhysics();

            btTransform trans;
            fallRigidBody->getMotionState()->getWorldTransform(trans);

            std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
    }

    return 0;
}