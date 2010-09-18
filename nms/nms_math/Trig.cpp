#define _USE_MATH_DEFINES
#include <cmath>
#include "Trig.h"




double nmsTrig::degToRad (double d) {
return d * M_PI / 180;
}

double nmsTrig::radToDeg (double r) {
return r * 180/ M_PI;
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