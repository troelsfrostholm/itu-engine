#ifdef __EXP_NMS_GL
#    define MEMBER_THREAD_D __declspec(dllexport)
#else
#    define MEMBER_THREAD_D __declspec(dllimport)
#endif

#ifndef SDL_MEMBER_THREAD
#define SDL_MEMBER_THREAD

#include <boost/shared_ptr.hpp>
#include "SDL.h"

template <typename T>
struct SDL_ThreadProxy
{
    typedef int (T::*TFunction)();
    SDL_ThreadProxy(T * instance, TFunction function)
        : _instance(instance)
        , _function(function)
    {}
    static int run(SDL_ThreadProxy<T> * _this)
    {
        return ((_this->_instance)->*(_this->_function))();
    }

private:
    T * _instance;
    TFunction _function;
};

template <typename T>
SDL_Thread * SDL_CreateMemberThread(T * instance, int (T::*function)())
{
	boost::shared_ptr<SDL_ThreadProxy<T>> proxy( new SDL_ThreadProxy<T>(instance, function) );
    //SDL_ThreadProxy<T> proxy(instance, function);
    typedef int (*SDL_ThreadFunction)(void *);
    return SDL_CreateThread(reinterpret_cast<SDL_ThreadFunction>(SDL_ThreadProxy<T>::run), proxy); 
}

#endif