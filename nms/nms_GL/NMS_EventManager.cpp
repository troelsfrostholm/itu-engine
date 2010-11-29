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
	idleCallback = &NoIdleEvent;
}

NMS_EventManager &NMS_EventManager::getInstance()
{
	if(!singleton) {
		singleton = new(STATIC_ALLOC, MEM_PERSISTENT) NMS_EventManager();
	}
	return *singleton;
}

void NMS_EventManager::destroy (void) {
	if (singleton) {
		delete singleton;
		singleton = 0;
	}
}

void NMS_EventManager::bindKeyPress(SDLKey key, string action)
{
	pressKeyMap[key] = action;
}

void NMS_EventManager::bindKeyHold(SDLKey key, string action)
{
	holdKeyMap[key] = action;
}
	
void NMS_EventManager::bindAction(string action, boost::function1<void, int> callback)
{
	actionMap[action] = callback;
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
	runHeldDownActions();
	idleCallback( 0 );
}

void NMS_EventManager::handleEvent(SDL_Event event)
{
	switch( event.type ) {

		// A key has been released
	case SDL_KEYUP:
		switch( event.key.keysym.sym ) 
		{ 
		  case SDLK_ESCAPE:
			  quitCallback(0);
			   break;
		  default:
			  if(holdKeyMap.count(event.key.keysym.sym) == 1)
				  keyHeldDown[event.key.keysym.sym] = false;
			  break;
		} 
		break;

	// A key has been pressed
	case SDL_KEYDOWN:
		switch( event.key.keysym.sym ) 
		{ 
		  case SDLK_ESCAPE:
			  quitCallback(0);
			   break;
		  default:
			  if(pressKeyMap.count(event.key.keysym.sym) == 1)
				  runAction(pressKeyMap[event.key.keysym.sym]);
			  if(holdKeyMap.count(event.key.keysym.sym) == 1)
				  keyHeldDown[event.key.keysym.sym] = true;
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

	default:
		break;
	}
}

void NMS_EventManager::runAction(string action)
{
	if(actionMap.count(action) == 1)
	{
		actionMap[action](0);
	}
}

void NMS_EventManager::runHeldDownActions()
{
	//iterate through keyHeldDown and run actions
	std::map<SDLKey, bool>::const_iterator itr;
	for(itr = keyHeldDown.begin(); itr != keyHeldDown.end(); ++itr)
	{
		if(itr->second) {
			runAction(holdKeyMap[itr->first]);
		}
	}
}