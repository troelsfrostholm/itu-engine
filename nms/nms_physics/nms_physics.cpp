#include "nms_physics.h"
#include <iostream>

void nms_physics::InitNx()
{
    // Create the physics SDK
    	// Create the physics SDK
    gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
    if (!gPhysicsSDK)  return;

	// Set the physics parameters
	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01);

	// Set the debug visualization parameters
	gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);

    // Create the scene
    NxSceneDesc sceneDesc;
 	sceneDesc.simType = NX_SIMULATION_SW;
	sceneDesc.gravity.set(0.0f, -9.8f, 0.0f);
    gScene = gPhysicsSDK->createScene(sceneDesc);

	
	// Create the default material
	NxMaterial* defaultMaterial = gScene->getMaterialFromIndex(0); 
	defaultMaterial->setRestitution(0.5);
	defaultMaterial->setStaticFriction(0.5);
	defaultMaterial->setDynamicFriction(0.5);

	debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamicsWorld->setDebugDrawer(&debugDrawer);

}

NxScene* nms_physics::getScene()
{
	return gScene;
}

void nms_physics::StartPhysics()
{
    // Update the time step
    NxReal deltaTime = 1.0f/60.0f;

    // Start collision and dynamics for delta time since the last frame
    gScene->simulate(deltaTime);
    gScene->flushStream();
}

void nms_physics::GetPhysicsResults()
{
	// Get results from gScene->simulate(gDeltaTime)
	while (!gScene->fetchResults(NX_RIGID_BODY_FINISHED, false));
}

void nms_physics::exitPhysics()
{
	if (gScene)
	{
		GetPhysicsResults();  // Make sure to fetchResults() before shutting down
		gPhysicsSDK->releaseScene(*gScene);
	}
	if (gPhysicsSDK)  gPhysicsSDK->release();
}

void nms_physics::simulatePhysics()
{
	StartPhysics();
	GetPhysicsResults();
}

nms_physics::nms_physics()
{
	InitNx();
}

nms_physics::~nms_physics()
{
	exitPhysics();
}
	dt = dt / 1000000.f; //Convert microseconds to seconds