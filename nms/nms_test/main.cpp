#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "SDL.h"

#define BOOST_TEST_MODULE MathTest
#include <iostream>
#include <NMS_Thread.h>

#include <boost/test/unit_test.hpp>

using namespace std;

/*class Thread
{
private:
	SDL_Thread *thread;

public:
	Thread()
	{
		thread = NULL;
	}

	virtual int run() = 0;

	void start()
	{
		assert(!thread);
		thread = SDL_CreateThread(&threadProxy, this);
		if(!thread)
		{
			throw std::runtime_error(SDL_GetError());
		}
	}
};

int threadProxy( void *pointer )
{
	try 
	{
		Thread &thread = *static_cast<Thread*>(pointer);
		thread.run();
		return 0;
	}
	catch( const std::exception &e )
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
	catch( ... )
	{
		std::cerr << "Unknown object thrown\n";
		return 2;
	}
}*/

class X : public Thread
{
	int run()
	{
		cout << "X is running" << endl;
		return 0;
	}
};


BOOST_AUTO_TEST_CASE( sdl_threads )
{

	
	//Thread t = Thread();
	//t.start();
	//boost::shared_ptr<Thr> p(new Thr());
	//SDL_CreateThread(&x, NULL);
}