#include "NMS_3DObject.h"
#include "Vector.h"

#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace std;

#define _CRTDBG_MAP_ALLOC

BOOST_AUTO_TEST_SUITE( Object3D );

BOOST_AUTO_TEST_CASE( Object3D_Creation )
{
	NMS_3DObject newObject = NMS_3DObject::NMS_3DObject();
	Vector test = Vector();
	BOOST_CHECK_EQUAL( newObject.getPosVector(),test);
	BOOST_CHECK_EQUAL( newObject.getDirVector(),test);
}

BOOST_AUTO_TEST_CASE( Object3D_DataAdded )
{
	NMS_3DObject newObject = NMS_3DObject::NMS_3DObject();
	Vector data= Vector();
	data[1]=1;
	data[2]=2;
	data[3]=3;
	newObject.setPosVector(&data);
	Vector returned = newObject.getPosVector();
	BOOST_CHECK_EQUAL( data,returned );
}
BOOST_AUTO_TEST_SUITE_END();