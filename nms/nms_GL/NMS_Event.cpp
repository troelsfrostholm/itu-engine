#include "NMS_Event.h"

NMS_Event *NMS_Event::singleton = 0;

NMS_Event::NMS_Event()
{
}

NMS_Event &NMS_Event::getInstance()
{
	if(!singleton) {
		singleton = new NMS_Event();
	}
	return *singleton;
}

void NMS_Event::destroy (void) {
	if (singleton) {
		delete singleton;
		singleton = 0;
	}
}

void NMS_Event::processEvents()
{
	SDL_Event event;
	// Take an event from the poll
	while( SDL_PollEvent( &event ) ) {
		switch( event.type ) {
		// A key has been pressed
		case SDL_KEYDOWN:
			switch( event.key.keysym.sym ) 
			{ 
			  case SDLK_ESCAPE:
				  quitCallback(0);
				   break;
			}
			break;
		// A key has been released
		case SDL_KEYUP:
			switch( event.key.keysym.sym ) 
			{ 
			  case SDLK_ESCAPE:
				  quitCallback(0);
				   break;
			} 
			break;
		// The window has been closed
		case SDL_QUIT:
			quitCallback(0);
			break;
		}
	}
}