#include "nms_physics.h"
#include <iostream>

int main (void)
{
	nms_physics* physics = new nms_physics();
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

    btCollisionShape* fallShape = new btSphereShape(1);
	

    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	physics->addRBody(groundRigidBody);


    btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
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