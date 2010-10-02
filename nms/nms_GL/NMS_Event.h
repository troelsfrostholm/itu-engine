#ifdef __EXP_NMS_GL
#    define EVENT_D __declspec(dllexport)
#else
#    define EVENT_D __declspec(dllimport)
#endif

#ifndef NMS_EVENT_H
#define NMS_EVENT_H


#pragma warning( disable: 4251 )  //Used to disable this useless warning: http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html
//#include "callback.h"
#include "SDL.h"
#include <boost/function.hpp>
#include <list>

using namespace std;

#define NMS_EVENT	NMS_Event::getInstance()
#define DESTROY_EVENT	NMS_Event::destroy()

class  EVENT_D NMS_Event
{
private:
	static NMS_Event *singleton;
	list<SDL_Event> eventQueue;
	int maxQueueSize;
	SDL_mutex* eventQueueGuard;

	boost::function1<void, int> quitCallback;
	boost::function1<void, SDLKey> keyPressedCallback;	

	NMS_Event::NMS_Event();

public:
	static NMS_Event& NMS_Event::getInstance();
	static void destroy (void);
	void NMS_Event::pollEvents();
	void NMS_Event::processEvents();
	void NMS_Event::handleEvent(SDL_Event event);

	template <class T> 
	void onQuit(T* instance, void (T::*_callback)(int i))
	{
		quitCallback = std::bind1st(std::mem_fun(_callback), instance);
	}

	void onKeyPressed(void (_callback)(SDLKey keysym))
	{
		keyPressedCallback = _callback;
	}
};

#endif