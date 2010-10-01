#include "NMS_Thread.h"

Thread::Thread()
{
	thread = NULL;
}

void Thread::start()
{
	assert(!thread);
	thread = SDL_CreateThread(&threadProxy, this);
	if(!thread)
	{
		throw std::runtime_error(SDL_GetError());
	}
}

SDL_Thread* Thread::getThread()
{
	return thread;
}

int Thread::run()
{
	return 0;
}

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
}