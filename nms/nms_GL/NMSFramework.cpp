#include "NMSFramework.h"


NMSFramework::NMSFramework(){};

bool NMSFramework::NMSInit(int width,int height,int bpp,char* windowTitle,bool fullscreen)
{
	//try to initialize SDL
	if( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) 
		{ return false; } 
	
	if (fullscreen) {
		flags = SDL_OPENGL | SDL_FULLSCREEN;
		SDL_ShowCursor(SDL_DISABLE);
	}
	else
	{
		flags = SDL_OPENGL;
		SDL_ShowCursor(SDL_DISABLE);
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
	glFrustum(    -0.5f,
                0.5f,
                -0.5f*(float)(height/width),
                0.5f*(float)(height/width),
                1.0f,
                500.0f);
	gluPerspective(60.0, (float)width/(float)height, 1.0, 1024);
	camera=NMSCameraFPS::NMSCameraFPS();
	camera.setPos(Vector(0,0,-5.0f));
	camera.setSpeed(0);
	camera.setSlideSpeed(0);
	running=true;
	return true;
}

void NMSFramework::NMSQuit()
{
	running=false;
	SDL_Quit();
}
	
void NMSFramework::NMSLoadTexture(char* fileName,int id)
{
	GLubyte TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};
	GLubyte TGAcompare[12];
	GLubyte header[6];
	FILE* file = fopen(fileName,"rb");
	fread(TGAcompare,1,sizeof(TGAcompare),file);
	if (memcmp(TGAheader,TGAcompare,sizeof(TGAheader)) != 0 )
	{
		fclose(file);
		printf("The file %s is not in a valid .TGA format\n",fileName);
		exit(-1);
	}
	fread(header,1,sizeof(header),file);
	int width = header[1]*256 + header[0];
	int height = header[3]*256 + header[2];
	if (width <=0 || height <=0 || header[4]!=24 ) {
		fclose(file);
		printf("The file %s is not in a valid .TGA format\n",fileName);
		exit(-1);
	}

	int bytesPerPixel = header[4]/8;
	int imageSize = width * height * bytesPerPixel;
	GLubyte* imageData = new GLubyte[imageSize];
	fread(imageData,1,imageSize,file);
	fclose(file);

	for(int i=0; i<(int)imageSize; i+=bytesPerPixel) {
		GLubyte temp = imageData[i];
		imageData[i] = imageData[i+2];
		imageData[i+2] = temp;
	}

	glBindTexture(GL_TEXTURE_2D,id);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
}

void NMSFramework::CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;
	static float lastTime = 0.0f;
	static char strCaption[80] = {0};
	float currentTime = SDL_GetTicks() * 0.001f;
	++framesPerSecond;
	if( currentTime - lastTime > 1.0f )
	{
		lastTime = currentTime;
		sprintf(strCaption,"Frames per Second: %d",int(framesPerSecond));
		SDL_WM_SetCaption(strCaption,NULL);
		framesPerSecond = 0;
	}
}






//CAMERA CONTROLELR FUNCTION DEFINITIONS
NMSCameraController::NMSCameraController()
{
	init();
}

NMSCameraController::~NMSCameraController()
{
}

void NMSCameraController::init()
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

void NMSCameraController::recalcAxes()
{
	Quaternion qFrame; //Quaternion calculated on this frame
	Matrix m;

	static float f2PI = 2*PI;

	//Keep into the range of 360 degrees to avoid overflow
	//Check on X
	if(fRotX>360.0f)
		fRotX-=360.0f;
	else
	  if(fRotX<-360.0f)
	    fRotX+=360.0f;

	//Check on Y
	if(fRotY>360.0f)
		fRotY-=360.0f;
	else
	  if(fRotY<-360.0f)
	    fRotY+=360.0f;

	//Check on Z
	if(fRotZ>360.0f)
		fRotZ-=360.0f;
	else
	  if(fRotZ<-360.0f)
	    fRotZ+=360.0f;

	qFrame.createFromAngles(fRotX,fRotY,fRotZ);

	qRot *= qFrame;

	m=qRot.getMatrix();

	vRight=m.getCol(1);
	vUp=m.getCol(2);
	vDir=m.getCol(3);
}

Matrix NMSCameraController::returnViewMatrix()
{
	Matrix toBeReturned=Matrix();
	toBeReturned.setRow(1,vRight);
	toBeReturned.setRow(2,vUp);
	toBeReturned.setRow(3,vDir);
	toBeReturned.setCol(4,vPosition);
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

	//Keep into the range of 360 degrees to avoid overflow
	//Check on Y
	if(fRotY>360.0f)
		fRotY-=360.0f;
	else
	  if(fRotY<-360.0f)
	    fRotY+=360.0f;

	//Check on X, keep on 80 degrees to avoid gimbal lock without using quaternions
	if(fRotX>80.0f)
		fRotX-=80.0f;
	else
	  if(fRotX<-80.0f)
	    fRotX+=80.0f;

	vRight=Vector(1.0f,0.0f,0.0f);
	vUp=Vector(0.0f,1.0f,0.0f);
	vDir=Vector(0.0f,0.0f,1.0f);

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

	recalcAxes();

	vVel=vDir*fSpeed*fET;
	vTemp=vRight*fSlide*fET;
	vPosition+=vVel+vTemp;
}