#include "Vector.h"

namespace nms {

	namespace math {

		// Test if first value approximates second value by an error margin less than error
		 bool approximates(float val1, float val2, float error);

		// For vectors this is done element by element
		 bool approximates(Vector val1, Vector val2, float error);

		// And for matrices as well
		 bool approximates(Matrix m1, Matrix m2, float error);

	};
};