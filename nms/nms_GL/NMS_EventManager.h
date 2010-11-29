#ifdef __EXP_NMS_GL
#    define EVENT_MANAGER_D __declspec(dllexport)
#else
#    define EVENT_MANAGER_D __declspec(dllimport)
#endif

#ifndef NMS_EVENT_MANAGER_H
#define NMS_EVENT_MANAGER_H

#pragma warning( disable: 4251 )  //Used to disable this useless warning: http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html


//#include "callback.h"
#include "SDL.h"
#include <boost/function.hpp>
#include <list>
#include <map>

using namespace std;

#define NMS_EVENT_MANAGER	NMS_EventManager::getInstance()
#define NMS_DESTROY_EVENT_MANAGER	NMS_EventManager::destroy()

class  EVENT_MANAGER_D NMS_EventManager
{
private:
	static NMS_EventManager *singleton;
	list<SDL_Event> eventQueue;
	int maxQueueSize;
	SDL_mutex* eventQueueGuard;

	boost::function1<void, int> quitCallback;
	boost::function1<void, int> pauseCallback;
	boost::function1<void, int> idleCallback;
	boost::function2<void, int, int> mouseMovedCallback;

	map<SDLKey, string> pressKeyMap; //Maps key down events to actions
	map<SDLKey, string> holdKeyMap;  //Maps key held down events to actions
	map<string, boost::function1<void, int>> actionMap;  //Maps actions to callback 

	map<SDLKey, bool> keyHeldDown;
	NMS_EventManager::NMS_EventManager();

public:
	static NMS_EventManager& NMS_EventManager::getInstance();
	static void destroy (void);
	void bindKeyPress(SDLKey key, string action);
	void bindKeyHold(SDLKey key, string action);
	void bindAction(string action, boost::function1<void, int> callback);
	void runAction(string action);
	void runHeldDownActions();

	void NMS_EventManager::pollEvents();
	void NMS_EventManager::processEvents();
	void NMS_EventManager::handleEvent(SDL_Event event);

	template <class T> 
	void onQuit(T* instance, void (T::*_callback)(int i))
	{
		quitCallback = std::bind1st(std::mem_fun(_callback), instance);
	}

	template <class T>
	void onPause(T* instance, void (T::*_callback)(int i))
	{
		pauseCallback = std::bind1st(std::mem_fun(_callback), instance);
	}

	void onIdle(void (_callback)(int i))
	{
		idleCallback = _callback;
	}

	void onMouseMoved(void (_callback)(int x, int y))
	{
		mouseMovedCallback = _callback;
	}
};

#endif