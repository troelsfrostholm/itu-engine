#define TRIG_D __declspec(dllexport)


#ifndef TRIG_H__
#define TRIG_H__

#include <cmath>
#define _USE_MATH_DEFINES

#define PI 3.14159265f;

class TRIG_D nmsTrig
{
public:
	static double degToRad (double d);
	static double radToDeg (double r);
	static float degToRad (float d);
	static float radToDeg (float r);
	static float cos (double x );
	static float sin (double x );
};
#endif