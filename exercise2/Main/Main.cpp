// Main.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <stdio.h>
#include <conio.h>

#include "stdafx.h"

#include "Engine.h"
// When you get this error 
// Error	1	fatal error C1083: Cannot open include file: 'Engine.h': No such file or directory	c:\git\itu-engine\exercise2\main\main.cpp	9	Main
// you need to open project properties->C++->Generel->additional include directories
// and add the folder where the .h is located

using namespace nms;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	//Engine started. Will terminate on its own.
	Engine::Start();

	//cout << "type something and presse enter to end" << endl;
	
	//char c;
	// Wait for keyboard press
	//cin >> c;
        

	
	return 0;
}

