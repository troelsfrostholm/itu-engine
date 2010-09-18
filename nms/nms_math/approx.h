#include "Vector.h"

namespace nms {

	namespace math {

		__declspec(dllexport) bool approximates(float val1, float val2, float error);
		__declspec(dllexport) bool approximates(Vector val1, Vector val2, float error);
		__declspec(dllexport) bool approximates(Matrix m1, Matrix m2, float error);

	};
};