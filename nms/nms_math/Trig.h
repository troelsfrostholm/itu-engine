#define TRIG_D __declspec(dllexport)


#ifndef TRIG_H__
#define TRIG_H__

#include <cmath>
#define _USE_MATH_DEFINES


class TRIG_D nmsTrig
{
public:
	template<class T>
	static int    signum(T v);
	static double degToRad (double d);
	static double radToDeg (double r);
	static float  degToRad (float d);
	static float  radToDeg (float r);
	static float  cos (double x );
	static float  sin (double x );
};
#endif