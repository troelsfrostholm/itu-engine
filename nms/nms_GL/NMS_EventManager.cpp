#include "NMS_EventManager.h"
#include "NMS_StaticAllocator.h"

NMS_EventManager *NMS_EventManager::singleton = 0;

void NoMouseEvent(int x, int y) { };
void NoKeyEvent(SDLKey key) { };
void NoIdleEvent(int i) { };

NMS_EventManager::NMS_EventManager()
{
	singleton = NULL;
	eventQueueGuard = SDL_CreateMutex();
	mouseMovedCallback = &NoMouseEvent;
	keyPressedCallback = &NoKeyEvent;
	keyReleasedCallback = &NoKeyEvent;
	idleCallback = &NoIdleEvent;
}

NMS_EventManager &NMS_EventManager::getInstance()
{
	if(!singleton) {
		singleton = new(STATIC_ALLOC, MEM_SINGLETON) NMS_EventManager();
	}
	return *singleton;
}

void NMS_EventManager::destroy (void) {
	if (singleton) {
		delete singleton;
		singleton = 0;
	}
}

void NMS_EventManager::pollEvents()
{
	SDL_Event event;
	SDL_LockMutex(eventQueueGuard);
	while( SDL_PollEvent( &event ) ) {
		eventQueue.push_back(event);
	}
	SDL_UnlockMutex(eventQueueGuard);
}

void NMS_EventManager::processEvents()
{
	SDL_LockMutex(eventQueueGuard);
	while( eventQueue.size() > 0 )  {
		handleEvent( eventQueue.front() );
		eventQueue.pop_front();
	}
	SDL_UnlockMutex(eventQueueGuard);
	idleCallback( 0 );
}

void NMS_EventManager::handleEvent(SDL_Event event)
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
		  default:
			  keyReleasedCallback( event.key.keysym.sym );
			  break;
		} 
		break;

	case SDL_MOUSEMOTION:
		int MouseX,MouseY;
		SDL_GetRelativeMouseState(&MouseX,&MouseY);
		mouseMovedCallback(MouseX, MouseY);
		break;
	// The window has been closed
	case SDL_QUIT:
		quitCallback(0);
		break;
	}
}