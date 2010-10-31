#define _USE_MATH_DEFINES
#include <cmath>
#include "Trig.h"

template<class T>
int nmsTrig::signum (T v) {
return (v > 0) - (v < 0)
}

double nmsTrig::degToRad (double d) {
return d * M_PI / 180;
}

double nmsTrig::radToDeg (double r) {
return r * 180/ M_PI;
}

float nmsTrig::degToRad (float d) {
	return (float) degToRad((double) d);
}

float nmsTrig::radToDeg (float r) {
	return (float) radToDeg((double) r);
}

float nmsTrig::sin (double x )
{
	int toBeRemoved=(int)x/360;
	x=x-toBeRemoved*360;
	if((0==x)||(180==x))
		return 0;
	else
	    return (float)std::sin(degToRad(x));
}


float nmsTrig::cos (double x )
{
	int toBeRemoved=(int)x/360;
	x=x-toBeRemoved*360;
	if((90==x)||(270==x))
		return 0;
	else
		return (float)std::cos(degToRad(x));
}