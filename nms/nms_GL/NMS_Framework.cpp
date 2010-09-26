#include "NMS_Framework.h"


NMS_Framework::NMS_Framework(){};

bool NMS_Framework::NMSInit(int width,int height,int bpp,char* windowTitle,bool fullscreen)
{
	//set callback for quitting
	NMS_EVENT.onQuit(this, &NMS_Framework::NMSQuit);

	//try to initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) 
		{ return false; } 
	
	if (fullscreen) {
		flags = SDL_OPENGL | SDL_FULLSCREEN;
		SDL_ShowCursor(SDL_DISABLE);
		SDL_WM_GrabInput(SDL_GRAB_ON);
	}
	else
	{
		flags = SDL_OPENGL;
		SDL_ShowCursor(SDL_DISABLE);
		SDL_WM_GrabInput(SDL_GRAB_ON);
	}
	SDL_WM_SetCaption(windowTitle,NULL);  //Set the name of the window
	SDL_SetVideoMode(width, height, bpp, flags); //Set the window mode
	glEnable(GL_TEXTURE_2D); //Initialize OpenGl and texture mapping
	glViewport(0, 0, width, height); // Set the dimensions of the viewport
	glMatrixMode(GL_PROJECTION);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLoadIdentity();
	/*glFrustum(    -0.5f,
                0.5f,
                -0.5f*(float)(height/width),
                0.5f*(float)(height/width),
                1.0f,
                500.0f);*/
	gluPerspective(60.0, (float)width/(float)height, 1.0, width);
	camera=NMSCameraFPS::NMSCameraFPS();
	camera.setPos(Vector(0,0,-5.0f));
	camera.setSpeed(0);
	camera.setSlideSpeed(0);
	running=true;
	return true;
}

void NMS_Framework::NMSQuit(int i)
{
	running=false;
	//Be sure to remove all the textures we have loaded from the memory!
	NMS_TEXTUREMANAGER.FreeAll();
	SDL_Quit();
}

void NMS_Framework::CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;
	static float lastTime = 0.0f;
	static char strCaption[80] = {0};
	static size_t strCaptionSize = 80*sizeof(char);
	float currentTime = SDL_GetTicks() * 0.001f;
	++framesPerSecond;
	if( currentTime - lastTime > 1.0f )
	{
		lastTime = currentTime;
		sprintf_s(strCaption,strCaptionSize,"Frames per Second: %d",int(framesPerSecond));
		SDL_WM_SetCaption(strCaption,NULL);
		framesPerSecond = 0;
	}
}

void NMS_Framework::run()
{
	while(running)
	{
		NMS_EVENT.processEvents();
		//render();
	}
}

//CAMERA CONTROLELR FUNCTION DEFINITIONS

void NMSCameraFPS::init()
{
	vPosition=Vector(0,0,0);
	vRight=Vector(1,0,0);
	vUp=Vector(0,1,0);
	vDir=Vector(0,0,1);
	vVel=Vector(0,0,0);

	fRotX=0;
	fRotY=0;
	fRotZ=0;

	fPitchSpd=0;
	fYawSpd=0;
	fRollSpd=0;

	qRot=Quaternion(0,0,0,1);
}


NMSCameraController::NMSCameraController()
{
}

NMSCameraController::~NMSCameraController()
{
}



Matrix NMSCameraController::returnViewMatrix()
{
	Matrix toBeReturned=Matrix();
	toBeReturned.setRow(1,vRight);
	toBeReturned.setRow(2,vUp);
	toBeReturned.setRow(3,vDir);
	Matrix trans=Matrix();
	trans.translate(vPosition);
	toBeReturned*=trans;
	toBeReturned(4,4)=1.0f;
	return toBeReturned;
}



//CAMERA FPS FUNCTION DEFINITIONS
NMSCameraFPS::NMSCameraFPS()
{
	init();
}

NMSCameraFPS::~NMSCameraFPS()
{
}

void NMSCameraFPS::setRotation(float rx, float ry, float rz)
{
	fRotX=rx;
	fRotY=ry;
	fRotZ=rz;
	recalcAxes();
}

Vector NMSCameraFPS::getRotation()
{
	return Vector(fRotX,fRotY,fRotZ);
}

void NMSCameraFPS::recalcAxes()
{
	Matrix m;

	vRight=Vector(1.0f,0.0f,0.0f);
	vUp=Vector(0.0f,1.0f,0.0f);
	vDir=Vector(0.0f,0.0f,1.0f);

	//Keep into the range of 360 degrees to avoid overflow
	//Check on Y
	if(fRotY>360.0f)
		fRotY-=360.0f;
	else
	  if(fRotY<-360.0f)
	    fRotY+=360.0f;

	if (fRotX>90.0f)
        fRotX = 90.0f;
    else if (fRotX<-90.0f)
        fRotX = -90.0f;

	//rotate around Y
	m.rotV(fRotY,vUp);
	vRight*=m;
	vDir*=m;

	//rotate around X 
	m.rotV(fRotX,vRight);
	vUp*=m;
	vDir*=m;


	//Correct rounding errors
	vDir=vDir.normal();
	vRight=vUp%vDir;
	vRight=vRight.normal();
	vUp=vDir%vRight;
	vUp=vUp.normal();
}

void NMSCameraFPS::UpdateCamera(float fET)
{
	Vector vTemp;
	fRotX+=fPitchSpd*fET;
	fRotY+=fYawSpd*fET;
	fRotZ+=fRollSpd*fET;

	vVel=vDir*fSpeed*fET;
	vTemp=vRight*fSlide*fET;
	vPosition+=vVel+vTemp;
	recalcAxes();
}