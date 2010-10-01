#include "NMS_Engine.h"

NMS_Framework engine;

bool InitEngine(int width,int height,int bpp,char* windowTitle,bool fullscreen)
{
	engine = NMS_Framework();
	return engine.NMSInit(width, height, bpp, windowTitle, fullscreen);
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	Mesh m = Mesh();
	m.render();

	//engine.render();
	SDL_GL_SwapBuffers();
}

void StartEngine()
{
	engine.running = true;
	while(engine.running)   {
		//gameloop
		NMS_EVENT.processEvents();
		Render();
	}
	engine.cleanup();
}

void QuitEngine()
{
	engine.running = false;
}
