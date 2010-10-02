#include "NMS_Event.h"

NMS_Event *NMS_Event::singleton = 0;

NMS_Event::NMS_Event()
{
	singleton = NULL;
	eventQueueGuard = SDL_CreateMutex();
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

void NMS_Event::pollEvents()
{
	SDL_Event event;
	SDL_LockMutex(eventQueueGuard);
	while( SDL_PollEvent( &event ) ) {
		eventQueue.push_back(event);
	}
	SDL_UnlockMutex(eventQueueGuard);
}

void NMS_Event::processEvents()
{
	SDL_LockMutex(eventQueueGuard);
	while( eventQueue.size() > 0 )  {
		handleEvent( eventQueue.front() );
		eventQueue.pop_front();
	}
	SDL_UnlockMutex(eventQueueGuard);
	//idleCallback( 0 );
}

void NMS_Event::handleEvent(SDL_Event event)
{
	switch( event.type ) {
	// A key has been pressed
	case SDL_KEYDOWN:
		switch( event.key.keysym.sym ) 
		{ 
		  case SDLK_ESCAPE:
			  quitCallback(0);
			   break;
		  default:
			  keyPressedCallback( event.key.keysym.sym );
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