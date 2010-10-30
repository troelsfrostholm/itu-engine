#include "NMS_StackBasedAllocator.h"
#include "NMS_StaticAllocator.cpp"
#include <boost/test/unit_test.hpp>
#include <iostream>

using namespace std;

BOOST_AUTO_TEST_SUITE( stack_allocator );

BOOST_AUTO_TEST_CASE( unaligned_allocation )
{
	StackBasedAllocator s = StackBasedAllocator(12);
	int * a = (int *) s.allocUnaligned(sizeof(int));
	char * b = (char *) s.allocUnaligned(sizeof(char));
	int * c = (int *) s.allocUnaligned(sizeof(int));
	
	*a = 1;
	*b = 'a';
	*c = 2;
	
	BOOST_CHECK(*a == 1 && *b == 'a' && *c == 2);
	BOOST_CHECK((U32)b - (U32)a == sizeof(int));
	BOOST_CHECK((U32)c - (U32)b == sizeof(char));
}

BOOST_AUTO_TEST_CASE( aligned_allocation )
{
	//Right now memory is aligned by just pushing every new allocation forward so
	//the next address % alignment == 0. 
	//We should probably do something less memory consuming... ;)
	StackBasedAllocator s = StackBasedAllocator(37);
	int * a = (int *) s.allocAligned(sizeof(int), 16);
	int * b = (int *) s.allocAligned(sizeof(int), 16);
	BOOST_CHECK((U32)a % 16 == 0);
	BOOST_CHECK((U32)b % 16 == 0);
	cout << "address of a : " << a << endl;
	cout << "address of b : " << b << endl;
	cout << "address of marker : " << (void *) s.getMarker() << endl;
}

BOOST_AUTO_TEST_CASE( free_and_overwrite )
{
	StackBasedAllocator s = StackBasedAllocator(30);
	int * a = (int *) s.allocUnaligned(sizeof(int));
	U32 marker = s.getMarker();
	int * b = (int *) s.allocUnaligned(sizeof(int));
	int * c = (int *) s.allocUnaligned(sizeof(int));
	*a  = 1;
	*b = 2;
	*c = 3;
	BOOST_CHECK(*a==1&&*b==2&&*c==3);
	s.freeToMarker(marker);
	int * d = (int *) s.allocUnaligned(sizeof(int));
	char * e = (char *) s.allocUnaligned(sizeof(char));
	int * f = (int *) s.allocUnaligned(sizeof(int));
	*d = 4;
	*e = 'e';
	*f = 5;
	BOOST_CHECK(*d==4&&*e=='e'&&*f==5);
	BOOST_CHECK(*b==*d&&*c!=3);
	BOOST_CHECK((U32)c==(U32)e);
}

BOOST_AUTO_TEST_CASE( exceed_allocator_size )
{
	StackBasedAllocator s = StackBasedAllocator(8);
	int * a = (int *) s.allocUnaligned(sizeof(int)*2);
	a[0]=1;
	a[1]=2;
	BOOST_CHECK(a[0]==1&&a[1]==2);
	bool exception_thrown = false;
	bool with_correct_message = false;
	try {
		int * b = (int *) s.allocUnaligned(sizeof(int));
	}
	catch(char* msg) {
		exception_thrown = true;
		if(strcmp(msg, "Allocator size exceeded")==0)
			with_correct_message = true;
	}
	BOOST_CHECK(exception_thrown);
	BOOST_CHECK(with_correct_message);
}

BOOST_AUTO_TEST_CASE( new_and_delete )
{
	StackBasedAllocator * s = new StackBasedAllocator(50);
	U32 initial_marker = s->getMarker();
	int * i = new(s, 0) int(1);
	U32 marker_after_i = s->getMarker();
	BOOST_CHECK_EQUAL((U32) i, initial_marker);
	BOOST_CHECK_EQUAL(marker_after_i - initial_marker, 4);
	delete s;
	s = NULL;
}

BOOST_AUTO_TEST_CASE( memory_tracking )
{
	StackBasedAllocator * s = new StackBasedAllocator(50);
	int * i = new(s, 0) int(1);
	int * j = new(s, 1) int(2);
	BOOST_CHECK_EQUAL(s->currentMemoryUsage(0), 4);
	BOOST_CHECK_EQUAL(s->currentMemoryUsage(1), 4);
	for(int k = 2; k < MemoryAllocator::NUM_CATEGORIES; k++) {
		BOOST_CHECK_EQUAL(s->currentMemoryUsage(k), 0);
	}
	delete s;
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_AUTO_TEST_SUITE( static_allocator );

BOOST_AUTO_TEST_CASE( allocating_into_several_stack_based_allocators )
{
	//int * i = new(STATIC_ALLOC, 0) int(1);
}

BOOST_AUTO_TEST_SUITE_END();