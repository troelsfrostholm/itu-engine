#ifndef TRIG_H__
#define TRIG_H__

#include <cmath>
#define _USE_MATH_DEFINES

namespace nmsTrig
{

__declspec(dllexport) double radians (double d);
__declspec(dllexport) double degrees (double r);
__declspec(dllexport) float cos (double x );
__declspec(dllexport) float sin (double x );

};
#endif