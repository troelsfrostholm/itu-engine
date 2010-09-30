#include "NMS_Framework.h"
#include "MD2Model.h"
#include "NMS_Audio.h"

#include <cmath>

#pragma warning( disable: 4244 )

#define WIDTH  640
#define HEIGHT  480
NMS_Framework engine = NMS_Framework();

MD2Model obj;
MD2Model obj2;


GLfloat	z=-10.0f;								// Depth Into The Screen


float animSpeed=1;




void ProcessEvents();

void DrawScene();

int main(int argc, char* argv[])
{ //Start SDL 
	engine.NMSInit(WIDTH,HEIGHT,16,"Nemesis Engine",false);
	NMS_Audio audioEngine;
	ALfloat sourcePos[] = {0.0f,0.0f,0.0f};
	ALfloat sourceVel[] = {0.0f,0.0f,0.0f};
	audioEngine.LoadWav("test.wav","test",sourcePos,sourceVel,1.0f,5.0f,true);
	audioEngine.playSound("test");
	audioEngine.LoadWav("test2.wav","test2",sourcePos,sourceVel,1.0f,5.0f,true);
	audioEngine.playSound("test2");
	engine.light.Enable(false);
	LightSource light0 = LightSource();
	light0.setLightNumber(GL_LIGHT0);
	light0.setLightValue(&Vector(1,1,1,0));
	light0.setPosVector(&Vector(0,-50,0,1));
	engine.light.defineLight(light0);
	
	//engine.light.setGlobalAmbient(&Vector(1.0,1.0,1.0,1.0));
	obj.LoadModel("models/drfreak/drfreak.md2");
	obj.LoadSkin("models/drfreak/drfreak.tga");
	obj.SetAnim(BOOM);
	while(true)
	{
		ProcessEvents(); // elabora gli eventi
		if(engine.running)
		{
			DrawScene(); // Draw the scene
			engine.CalculateFrameRate(); // Calculate the framerate
		}
		else
			break;
	}
	engine.cleanup();
	return 0; 
};



void ProcessEvents()
{
	SDL_Event event;
	// Take an event from the poll
	while( SDL_PollEvent( &event ) ) {
		switch( event.type ) {
		// A key has been pressed
		case SDL_KEYDOWN:
			switch( event.key.keysym.sym ) 
			{ case SDLK_UP:
				   engine.camera.setRSpeedX(+90.0f);
				   //xspeed-=0.1f; 
			       break; 
			  case SDLK_DOWN:engine.camera.setRSpeedX(-90.0f); 
				   break; 
			  case SDLK_LEFT:engine.camera.setRSpeedY(-90.f);
				   break; 
			  case SDLK_RIGHT:engine.camera.setRSpeedY(+90.0f);
				   break; 
			  case SDLK_PAGEUP:z-=0.2f; 
				   break; 
		      case SDLK_PAGEDOWN:z+=0.2f; 
				   break; 
			  case SDLK_a:engine.camera.setSlideSpeed(+0.01f);
				   break;
			  case SDLK_d:engine.camera.setSlideSpeed(-0.01f);
				   break;
			  case SDLK_w:engine.camera.setSpeed(+0.01f);
				   break;
			  case SDLK_s:engine.camera.setSpeed(-0.01f);
				   break;
			  case SDLK_q:;
				  //xspeed=0.0;
				  //yspeed=0.0;
				   break;
			  case SDLK_ESCAPE:engine.NMSQuit(0);
				   break;
			} 
			break;
		// A key has been released
		case SDL_KEYUP:
			switch( event.key.keysym.sym ) 
			{ case SDLK_UP:engine.camera.setRSpeedX(0);
				   //xspeed-=0.1f; 
			       break; 
			  case SDLK_DOWN:engine.camera.setRSpeedX(0); 
				   break; 
			  case SDLK_LEFT:engine.camera.setRSpeedY(0);
				   break; 
			  case SDLK_RIGHT:engine.camera.setRSpeedY(0);
				   break; 
			  case SDLK_PAGEUP:z-=0.2f; 
				   break; 
		      case SDLK_PAGEDOWN:z+=0.2f; 
				   break; 
			  case SDLK_a:engine.camera.setSlideSpeed(0);
				   break;
			  case SDLK_d:engine.camera.setSlideSpeed(0);
				   break;
			  case SDLK_w:engine.camera.setSpeed(0);
				   break;
			  case SDLK_s:engine.camera.setSpeed(0);
				   break;
			  case SDLK_q:;
				  //xspeed=0.0;
				  //yspeed=0.0;
				   break;
			  case SDLK_ESCAPE:engine.NMSQuit(0);
				   break;
			} 
			break;
		// The window has been closed
		case SDL_MOUSEMOTION:
			{
				int MouseX,MouseY;
				Vector rot=engine.camera.getRotation();
				SDL_GetRelativeMouseState(&MouseX,&MouseY);
				//Pitch rotation, with the mouse is up and down
				//rot[NMS_X]=(MouseY-HEIGHT/2)*0.01f;
				rot[NMS_X]+=(MouseY)*0.01f;
				//Yaw rotation, with the mouse you can do it using the left right position
				rot[NMS_Y]+=(MouseX)*0.01f;
				
				engine.camera.setRotation(rot[NMS_X],rot[NMS_Y],rot[NMS_Z]);
			}
			break;
		case SDL_QUIT:
			engine.NMSQuit(0);
			break;
		}
	}
}


void DrawMD2Model()
{
	gluLookAt(-87.0, 45.5, 0, 0, 2, 0, 0.0, 1.0, 0.0);
	//animSpeed+=0.0008f;
	animSpeed=0;
	obj.DrawModel(animSpeed);
}

void DrawNet(GLfloat size, GLint LinesX, GLint LinesZ)
{
	glBegin(GL_LINES);
	for (int xc = 0; xc < LinesX; xc++)
	{
		glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
					0.0,
					size / 2.0);
		glVertex3f(	-size / 2.0 + xc / (GLfloat)(LinesX-1)*size,
					0.0,
					size / -2.0);
	}
	for (int zc = 0; zc < LinesX; zc++)
	{
		glVertex3f(	size / 2.0,
					0.0,
					-size / 2.0 + zc / (GLfloat)(LinesZ-1)*size);
		glVertex3f(	size / -2.0,
					0.0,
					-size / 2.0 + zc / (GLfloat)(LinesZ-1)*size);
	}
	glEnd();
}

void DrawSampleScene()
{
	glPushMatrix();
	glTranslatef(0.0f,0.0f,z);						    // Translate Into/Out Of The Screen By z
	glRotatef(0,1.0f,0.0f,0.0f);						// Rotate On The NMS_X Axis By xrot
	glRotatef(0,0.0f,1.0f,0.0f);						// Rotate On The NMS_Y Axis By yrot
	
	glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 1 (Front)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 2 (Front)
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Front)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 4 (Front)
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Back)
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 2 (Back)
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 3 (Back)
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 4 (Back)
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 1 (Top)
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 2 (Top)
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Top)
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 4 (Top)
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Bottom)
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 2 (Bottom)
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 3 (Bottom)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 4 (Bottom)
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	// Point 1 (Right)
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	// Point 2 (Right)
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	// Point 3 (Right)
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	// Point 4 (Right)
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	// Point 1 (Left)
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	// Point 2 (Left)
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	// Point 3 (Left)
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	// Point 4 (Left)
	glEnd();

	glColor3f(0.3f,0.3f,0.3f);
	glBegin(GL_QUADS);
	glVertex3f( 0,-1.0, 0);
	glVertex3f( 0,-1.0,10);
	glVertex3f(10,-1.0,10);
	glVertex3f(10,-1.0, 0);
	glEnd();

	glTranslatef(0.0f,0.8f,0.0f);

	glScalef(30.0,30.0,30.0);
	GLfloat size = 2.0;
	GLint LinesX = 30;
	GLint LinesZ = 30;
	
	GLfloat halfsize = size / 2.0;
	glColor3f(1.0,1.0,1.0);
	glPushMatrix();
		glTranslatef(0.0,-halfsize ,0.0);
		DrawNet(size,LinesX,LinesZ);
		glTranslatef(0.0,size,0.0);
		DrawNet(size,LinesX,LinesZ);
	glPopMatrix();
	glColor3f(0.0,0.0,1.0);
	glPushMatrix();
		glTranslatef(-halfsize,0.0,0.0);	
		glRotatef(90.0,0.0,0.0,halfsize);
		DrawNet(size,LinesX,LinesZ);
		glTranslatef(0.0,-size,0.0);
		DrawNet(size,LinesX,LinesZ);
	glPopMatrix();
	glColor3f(1.0,0.0,0.0);
	glPushMatrix();
		glTranslatef(0.0,0.0,-halfsize);	
		glRotatef(90.0,halfsize,0.0,0.0);
		DrawNet(size,LinesX,LinesZ);
		glTranslatef(0.0,size,0.0);
		DrawNet(size,LinesX,LinesZ);
	glPopMatrix();
		
	glFlush();

	glPopMatrix();
	
}

void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	engine.camera.UpdateCamera(1);
	Matrix view=engine.camera.returnViewMatrix();
	view=(~view);
	glMultMatrixf(view.returnPointer());
	
	DrawMD2Model();
	//DrawSampleScene();
	SDL_GL_SwapBuffers();
}