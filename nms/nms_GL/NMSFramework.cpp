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
	gluPerspective(60.0, (float)width/(float)height, 1.0, 1024.0);
	running=true;
	camera.cameraView=Matrix();
	camera.Position(0.0,0.0,-5.0,0.0,0.0);
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



//CAMERA FUNCTION DEFINITIONS
void NMSCamera::Position(float positionX,float positionY,float positionZ,
						 double pitchAngle,double yawAngle)
		{

			//TRANSPOSITION FROM THE POSITION
			//ROTATION AMONG X for the up vecctor
			//ROTATION AMONG Y for the view vector
			position=Vector(positionX,positionY,positionZ);
			rightVelocity=0;
			upVelocity=0;
			camPitch=0;
			camYaw=0;
			camRoll=0;
			directionVelocity=0;
		}

void NMSCamera::Move(float directionVelocity,float rightVelocity,float upVelocity)
{
	float temp1,temp2;
	temp1=camYaw;
	temp2=camPitch;
	camYaw = (camYaw/180*NUM_PI+NUM_PI/2); 
	camPitch = (camPitch/180*NUM_PI+NUM_PI/2);
	/*
	mPosition(1,4)+=float(sin(camYaw))*rightVelocity;//float(sin(camPitch))*directionVelocity ;
	mPosition(2,4)-=float(cos(camYaw))*directionVelocity;
	mPosition(3,4)+=float(sin(camPitch))*directionVelocity +float(cos(camYaw))*rightVelocity;
	*/
	camYaw=temp1;
	camPitch=temp2;
}

void NMSCamera::Rotate()
{
		//mUp.rotX(camPitch);
		//mView.rotY(camYaw);
}

void NMSCamera::updatePositions()
{
	up=Vector(0.0f,1.0f,0.0f);
	look=Vector(0.0f,0.0f,1.0f);
	right=Vector(1.0f,0.0f,0.0f);
	
	//Rotate around the Y axis
	Matrix rotation=Matrix();
	rotation.rotV(camYaw,up);

	look=look*rotation;
	right=right*rotation;
	
	//Rotate around the X axis
	rotation.rotV(camPitch,right);

	look=look*rotation;
	up=up*rotation;

	//Rotate around the Z axis NOT USED FOR THE FPSes
	rotation.rotV(camRoll,look);

	right=right*rotation;
	up=up*rotation;


	//Update the position
	position+=look*directionVelocity;    // Move in the look direction
	position+=right*rightVelocity;   // Move right (strafe)
	position+=up*upVelocity;      // Move up

	cameraView.setRow(1,right);
	cameraView.setRow(2,up);
	cameraView.setRow(3,look);
	cameraView.setCol(4,position*-1);
	cameraView(4,4)=1;
}
