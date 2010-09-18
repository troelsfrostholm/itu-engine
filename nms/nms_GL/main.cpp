#include "NMSFramework.h"

NMSFramework engine = NMSFramework();
bool keys[256];


GLfloat	xrot;									// NMS_X Rotation
GLfloat	yrot;									// NMS_Y Rotation
GLfloat xspeed;									// NMS_X Rotation Speed
GLfloat yspeed;									// NMS_Y Rotation Speed
GLfloat	z=-10.0f;								// Depth Into The Screen

BOOL	light;									// Lighting ON / OFF
BOOL	lp;									    // L Pressed?
BOOL	fp;									    // F Pressed?


GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; // Ambient Light Values

GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };	// Diffuse Light Values

GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f }; // Light Position

GLuint	filter;									// Which Filter To Use
GLuint	texture[3];								// Storage for 3 textures






void ProcessEvents();

void DrawScene();

int main(int argc, char* argv[])
{ //Start SDL 
	engine.NMSInit(640,480,16,"Nemesis Engine",false);
	engine.NMSLoadTexture("Texture.tga",5666);
	while(true)
	{
		ProcessEvents(); // elabora gli eventi
		if(engine.running)
		{
			//ReadKeyboard(); // legge la tastiera
			DrawScene(); // disegna la scena
			engine.CalculateFrameRate(); // calcola il frame rate
		}
		else
			break;
	}
	engine.NMSQuit();
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
			{ case SDLK_UP:xspeed-=0.1f; 
			       break; 
			  case SDLK_DOWN:xspeed+=0.1f; 
				   break; 
			  case SDLK_LEFT:yspeed-=0.1f;
				   break; 
			  case SDLK_RIGHT:yspeed+=0.1f;
				   break; 
			  case SDLK_PAGEUP:z-=0.2f; 
				   break; 
		      case SDLK_PAGEDOWN:z+=0.2f; 
				   break; 
			  case SDLK_a:engine.camera.setSlideSpeed(+0.01f);
				   break;
			  case SDLK_d:engine.camera.setSlideSpeed(-0.01f);
				   break;
			  case SDLK_w:engine.camera.setSpeed(+0.1f);
				   break;
			  case SDLK_s:engine.camera.setSpeed(-0.1f);
				   break;
			  case SDLK_q:xspeed=0.0;
						  yspeed=0.0;
				   break;
			  case SDLK_ESCAPE:engine.NMSQuit();
				   break;
			} 
			break;
		// A key has been released
		case SDL_KEYUP:
			switch( event.key.keysym.sym ) 
			{ case SDLK_UP:xspeed-=0.1f; 
			       break; 
			  case SDLK_DOWN:xspeed+=0.1f; 
				   break; 
			  case SDLK_LEFT:yspeed-=0.1f;
				   break; 
			  case SDLK_RIGHT:yspeed+=0.1f;
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
			  case SDLK_q:xspeed=0.0;
						  yspeed=0.0;
				   break;
			  case SDLK_ESCAPE:engine.NMSQuit();
				   break;
			} 
			break;
		// The window has been closed
		case SDL_MOUSEMOTION:
			{
				//int MouseX,MouseY;
				//SDL_GetRelativeMouseState(&MouseX,&MouseY);
				////MouseY=MouseY-240;
				////MouseX=MouseX-320;

				//engine.camera.setRSpeedX(MouseX*0.001f);
				//engine.camera.setRSpeedY(MouseY*0.001f);
				////The Y axis is inverted on the mouse!
				////engine.camera.clampCamera();
				//SDL_WarpMouse(320,240);
			}
			break;
		case SDL_QUIT:
			engine.NMSQuit();
			break;
		}
	}
}


void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	engine.camera.UpdateCamera(1);
	Matrix view=engine.camera.returnViewMatrix();
	glMultMatrixf((~(view)).returnPointer());

	//glMultMatrixf((~(engine.camera.mPosition)).returnPointer());
	//glMultMatrixf((~(engine.camera.mUp)).returnPointer());//Pitch;
	
	
	
	glPushMatrix();
	glTranslatef(0.0f,0.0f,z);						    // Translate Into/Out Of The Screen By z
	glRotatef(xrot,1.0f,0.0f,0.0f);						// Rotate On The NMS_X Axis By xrot
	glRotatef(yrot,0.0f,1.0f,0.0f);						// Rotate On The NMS_Y Axis By yrot
	
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

	glColor3f(.3,.3,.3);
	glBegin(GL_QUADS);
	glVertex3f( 0,-0.001, 0);
	glVertex3f( 0,-0.001,10);
	glVertex3f(10,-0.001,10);
	glVertex3f(10,-0.001, 0);
	glEnd();

	glBegin(GL_LINES);
	for(GLfloat i=0;i<=10;i++) {
		if (i==0) { glColor3f(.6,.3,.3); } else { glColor3f(.25,.25,.25); };
		glVertex3f(i,0,0);
		glVertex3f(i,0,10);
		if (i==0) { glColor3f(.3,.3,.6); } else { glColor3f(.25,.25,.25); };
		glVertex3f(0,0,i);
		glVertex3f(10,0,i);
	};
	glEnd();

	xrot+=xspeed;								// Add xspeed To xrot
	yrot+=yspeed;								// Add yspeed To yrot
	glPopMatrix();
	SDL_GL_SwapBuffers();
}

