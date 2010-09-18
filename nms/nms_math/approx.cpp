#include "approx.h"
#include "Vector.h"

bool nms::math::approximates(float val1, float val2, float error)
{
	return val1 < val2 + error &&
		   val1 > val2 - error;
}

bool nms::math::approximates(Vector val1, Vector val2, float error)
{
	for(int i=1; i<5; i++) {
		if(!approximates(val1[i], val2[i], error)) return false;
	}
	return true;
}

//ToDo: Implement approximates for Matrix
