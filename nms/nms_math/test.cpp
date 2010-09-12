#include <iostream>
#include "Matrix.h"

using namespace std;

//This is a test used to show the memory management, just debug and see how it works
Matrix prova(Matrix &m)
{
	Matrix temp=Matrix(3,3);
	temp=m;
	return temp;
}

int main()
{
	Matrix test= Matrix(4,4);
	test(1,1)=-2;
	test(1,2)=0;
	test(1,3)=3;
	test(1,4)=4;
	test(2,1)=1;
	test(2,2)=0;
	test(2,3)=1;
	test(2,4)=-2;
	test(3,1)=-1;
	test(3,2)=5;
	test(3,3)=4;
	test(3,4)=5;
	test(4,1)=-2;
	test(4,2)=3;
	test(4,3)=8; 
	test(4,4)=7;

	cout<<test<<'\n';

	//Memory Management test
	//test=prova(test);

	//Test matrix inversion and multiplication
	Matrix test2=!test;
	
	test=test*test2;

	Matrix I=Matrix();
	I(1,1)=5;
	I(2,1)=3;
	cout<<I<<'\n';
	Vector v=Vector(1,2,3,4);
	v=v*I;
	cout<<v<<'\n';
	

	cout<<test<<'\n';

	test.uScale(5.0f);

	cout<<test<<'\n';
	system("pause");
	return 0;
}