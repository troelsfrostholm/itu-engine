#include "Matrix4x4.h"
#include "approx.h"
#include "Quaternion.h"

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <cstring>

using namespace std;
using namespace nms::math;

BOOST_AUTO_TEST_SUITE( matrix4x4 );

Matrix4x4 factoryfunc()
{
	Matrix4x4 mat = Matrix4x4();
	return mat;
}

bool elmsEqual(const float * elm1, const float * elm2)
{
	return memcmp(elm1, elm2, 16) == 0;
}

BOOST_AUTO_TEST_CASE( construction )
{
	Matrix4x4 mat;
	const float * elms = mat.getElements();
	float expected[16] = { 1, 0, 0, 0,
	                       0, 1, 0, 0,
						   0, 0, 1, 0,
						   0, 0, 0, 1 };
	BOOST_CHECK( elmsEqual(elms, expected) );
	elms = factoryfunc().getElements();
	BOOST_CHECK( elmsEqual(elms, expected) );

	float createWith[16] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
	mat = Matrix4x4(createWith);
	elms = mat.getElements();
	BOOST_CHECK( elmsEqual(elms, createWith) );
}

BOOST_AUTO_TEST_CASE( lookup_write )
{
	Matrix4x4 mat;
	float expected[4][4] = 	{ 1, 0, 0, 0,
	                          0, 1, 0, 0,
	                          0, 0, 1, 0,
	                          0, 0, 0, 1  };
	float val, exp;
	for(int i=1; i<=4; i++)
	{
		for(int j=1; j<=4; j++)
		{
			
			BOOST_CHECK_EQUAL(mat(i, j), expected[i-1][j-1]);
		}
	}
	mat(2, 3) = 3.7;
	BOOST_CHECK(approximates(mat(2, 3), 3.7, 0.0001));
	float expt[16] =         { 1, 0, 0, 0,
	                          0, 1, 0, 0,
	                          0, 3.7, 1, 0,
	                          0, 0, 0, 1  };
	const float * elms = mat.getElements();
	BOOST_CHECK(elmsEqual(elms, expt));
}

BOOST_AUTO_TEST_SUITE_END();