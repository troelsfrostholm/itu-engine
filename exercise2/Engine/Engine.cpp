/*
Threads
http://www.codeproject.com/KB/threads/Threads_1.aspx
remember to set "Use Multi-Byte Character Set" under project properties->generel.
*/

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
//#include <gl\glaux.h>		// Header File For The Glaux Library //deprecated
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include "Engine.h"

using namespace std;

namespace nms
{
	HDC			hDC=NULL;		// Private GDI Device Context
	HGLRC		hRC=NULL;		// Permanent Rendering Context
	HWND		hWnd=NULL;		// Holds Our Window Handle
	HINSTANCE	hInstance;		// Holds The Instance Of The Application

	bool	keys[256];			// Array Used For The Keyboard Routine
	bool	active=TRUE;		// Window Active Flag Set To TRUE By Default
	bool	fullscreen=FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default

	GLfloat		rtri;						// Angle For The Triangle ( NEW )
	GLfloat		rquad;						// Angle For The Quad     ( NEW )

	LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

	bool engine_running=false;
	int world_update_thread_data = 1;
	
	HANDLE world_update_thread_handle = 0;
	HANDLE thread_handles[1];

	void HandleKeyboard()
	{
		if(_kbhit())
		{
			int ch = _getch();
			switch(ch)
			{
				//Terminate with keyboard hit 'ESC'.
				case 0x1B:
					engine_running=false;
					break;

				case 'w':
					rquad-=1.5f;
					break;
				
				case 's':
					rquad+=1.5f;
					break;

				case 'a':
					rtri-=1.5f;
					break;

				case 'd':
					rtri+=1.5f;
					break;
			}
		}
	}

	// Function for world update thread.
	DWORD WINAPI WorldUpdate( LPVOID lpParam ) 
	{
		cout << "Thread *WorldUpdate* started ..." << endl;
		
		while(engine_running)
		{
			HandleKeyboard();
		}

		cout << "Thread *WorldUpdate* ended ..." << endl;
		
		return 0;
	}

	int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
	{
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

		return TRUE;										// Initialization Went OK
	}

	int DrawGLTriangleAndCubeTestScene(GLvoid)									// Here's Where We Do All The Drawing
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		glLoadIdentity();									// Reset The Current Modelview Matrix
		
		glTranslatef(-1.5f,0.0f,-6.0f);					// Move Left 1.5 Units And Into The Screen 6.0
		glRotatef(rtri,0.0f,1.0f,0.0f);				// Rotate The Triangle On The Y axis ( NEW )

		glBegin(GL_TRIANGLES);						// Drawing Using Triangles
			glColor3f(1.0f,0.0f,0.0f);			// Red
			glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Front)
			glColor3f(0.0f,1.0f,0.0f);			// Green
			glVertex3f(-1.0f,-1.0f, 1.0f);			// Left Of Triangle (Front)
			glColor3f(0.0f,0.0f,1.0f);			// Blue
			glVertex3f( 1.0f,-1.0f, 1.0f);			// Right Of Triangle (Front)

			glColor3f(1.0f,0.0f,0.0f);			// Red
			glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Right)
			glColor3f(0.0f,0.0f,1.0f);			// Blue
			glVertex3f( 1.0f,-1.0f, 1.0f);			// Left Of Triangle (Right)
			glColor3f(0.0f,1.0f,0.0f);			// Green
			glVertex3f( 1.0f,-1.0f, -1.0f);			// Right Of Triangle (Right)

			glColor3f(1.0f,0.0f,0.0f);			// Red
			glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Back)
			glColor3f(0.0f,1.0f,0.0f);			// Green
			glVertex3f( 1.0f,-1.0f, -1.0f);			// Left Of Triangle (Back)
			glColor3f(0.0f,0.0f,1.0f);			// Blue
			glVertex3f(-1.0f,-1.0f, -1.0f);			// Right Of Triangle (Back)

			glColor3f(1.0f,0.0f,0.0f);			// Red
			glVertex3f( 0.0f, 1.0f, 0.0f);			// Top Of Triangle (Left)
			glColor3f(0.0f,0.0f,1.0f);			// Blue
			glVertex3f(-1.0f,-1.0f,-1.0f);			// Left Of Triangle (Left)
			glColor3f(0.0f,1.0f,0.0f);			// Green
			glVertex3f(-1.0f,-1.0f, 1.0f);			// Right Of Triangle (Left)

		glEnd();							// Finished Drawing The Triangle

	glLoadIdentity();									// Reset The Current Modelview Matrix
		glTranslatef(1.5f,0.0f,-6.0f);						// Move Right 1.5 Units And Into The Screen 6.0
		glRotatef(rquad,1.0f,0.0f,0.0f);					// Rotate The Quad On The X axis ( NEW )
		glColor3f(0.5f,0.5f,1.0f);		

		
		glBegin(GL_QUADS);						// Draw A Quad
			glColor3f(0.0f,1.0f,0.0f);			// Set The Color To Green
			glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Top)
			glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Top)
			glVertex3f(-1.0f, 1.0f, 1.0f);			// Bottom Left Of The Quad (Top)
			glVertex3f( 1.0f, 1.0f, 1.0f);			// Bottom Right Of The Quad (Top)

			glColor3f(1.0f,0.0f,1.0f);			// Set The Color To Orange
			glVertex3f( 1.0f,-1.0f, 1.0f);			// Top Right Of The Quad (Bottom)
			glVertex3f(-1.0f,-1.0f, 1.0f);			// Top Left Of The Quad (Bottom)
			glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Bottom)
			glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Bottom)

			glColor3f(1.0f,0.0f,0.0f);			// Set The Color To Red
			glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Front)
			glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Front)
			glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Front)
			glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Front)

			glColor3f(1.0f,1.0f,0.0f);			// Set The Color To Yellow
			glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Back)
			glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Back)
			glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Back)
			glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Back)

			glColor3f(0.0f,0.0f,1.0f);			// Set The Color To Blue
			glVertex3f(-1.0f, 1.0f, 1.0f);			// Top Right Of The Quad (Left)
			glVertex3f(-1.0f, 1.0f,-1.0f);			// Top Left Of The Quad (Left)
			glVertex3f(-1.0f,-1.0f,-1.0f);			// Bottom Left Of The Quad (Left)
			glVertex3f(-1.0f,-1.0f, 1.0f);			// Bottom Right Of The Quad (Left)

			glColor3f(1.0f,0.0f,1.0f);			// Set The Color To Violet
			glVertex3f( 1.0f, 1.0f,-1.0f);			// Top Right Of The Quad (Right)
			glVertex3f( 1.0f, 1.0f, 1.0f);			// Top Left Of The Quad (Right)
			glVertex3f( 1.0f,-1.0f, 1.0f);			// Bottom Left Of The Quad (Right)
			glVertex3f( 1.0f,-1.0f,-1.0f);			// Bottom Right Of The Quad (Right)
		glEnd();							// Done Drawing The Quad

		return TRUE;									// Everything Went OK
	}

	int DrawGLSceneGraphScene(GLvoid)									// Here's Where We Do All The Drawing
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		
		return TRUE;
	}


	GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
	{
		if (fullscreen)										// Are We In Fullscreen Mode?
		{
			ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
			ShowCursor(TRUE);								// Show Mouse Pointer
		}

		if (hRC)											// Do We Have A Rendering Context?
		{
			if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			{
				MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}

			if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
			{
				MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			}
			hRC=NULL;										// Set RC To NULL
		}

		if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
		{
			MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hDC=NULL;										// Set DC To NULL
		}

		if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
		{
			MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hWnd=NULL;										// Set hWnd To NULL
		}

		if (!UnregisterClass("OpenGL",hInstance))			// Are We Able To Unregister Class
		{
			MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
			hInstance=NULL;									// Set hInstance To NULL
		}
	}

	GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
	{
		if (height==0)										// Prevent A Divide By Zero By
		{
			height=1;										// Making Height Equal One
		}

		glViewport(0,0,width,height);						// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate The Aspect Ratio Of The Window
		gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
	}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			keys[wParam] = FALSE;					// If So, Mark It As FALSE
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}


	/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
	 *	title			- Title To Appear At The Top Of The Window				*
	 *	width			- Width Of The GL Window Or Fullscreen Mode				*
	 *	height			- Height Of The GL Window Or Fullscreen Mode			*
	 *	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
	 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/
	 
	BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
	{
		GLuint		PixelFormat;			// Holds The Results After Searching For A Match
		WNDCLASS	wc;						// Windows Class Structure
		DWORD		dwExStyle;				// Window Extended Style
		DWORD		dwStyle;				// Window Style
		RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
		WindowRect.left=(long)0;			// Set Left Value To 0
		WindowRect.right=(long)width;		// Set Right Value To Requested Width
		WindowRect.top=(long)0;				// Set Top Value To 0
		WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

		fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

		hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
		wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
		wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc Handles Messages
		wc.cbClsExtra		= 0;									// No Extra Window Data
		wc.cbWndExtra		= 0;									// No Extra Window Data
		wc.hInstance		= hInstance;							// Set The Instance
		wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
		wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
		wc.hbrBackground	= NULL;									// No Background Required For GL
		wc.lpszMenuName		= NULL;									// We Don't Want A Menu
		wc.lpszClassName	= "OpenGL";								// Set The Class Name

		if (!RegisterClass(&wc))									// Attempt To Register The Window Class
		{
			MessageBox(NULL,"Failed To Register The Window Class.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;											// Return FALSE
		}
		
		if (fullscreen)												// Attempt Fullscreen Mode?
		{
			DEVMODE dmScreenSettings;								// Device Mode
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
			dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
			dmScreenSettings.dmBitsPerPel	= bits;					// Selected Bits Per Pixel
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

			// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
			if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
			{
				// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
				if (MessageBox(NULL,"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","NeHe GL",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
				{
					fullscreen=FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
				}
				else
				{
					// Pop Up A Message Box Letting User Know The Program Is Closing.
					MessageBox(NULL,"Program Will Now Close.","ERROR",MB_OK|MB_ICONSTOP);
					return FALSE;									// Return FALSE
				}
			}
		}

		if (fullscreen)												// Are We Still In Fullscreen Mode?
		{
			dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
			dwStyle=WS_POPUP;										// Windows Style
			ShowCursor(FALSE);										// Hide Mouse Pointer
		}
		else
		{
			dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
			dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
		}

		AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

		// Create The Window
		if (!(hWnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
									"OpenGL",							// Class Name
									title,								// Window Title
									dwStyle |							// Defined Window Style
									WS_CLIPSIBLINGS |					// Required Window Style
									WS_CLIPCHILDREN,					// Required Window Style
									800, 400,								// Window Position
									WindowRect.right-WindowRect.left,	// Calculate Window Width
									WindowRect.bottom-WindowRect.top,	// Calculate Window Height
									NULL,								// No Parent Window
									NULL,								// No Menu
									hInstance,							// Instance
									NULL)))								// Dont Pass Anything To WM_CREATE
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Window Creation Error.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		static	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			bits,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			16,											// 16Bit Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};
		
		if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		ShowWindow(hWnd,SW_SHOW);						// Show The Window
		SetForegroundWindow(hWnd);						// Slightly Higher Priority
		SetFocus(hWnd);									// Sets Keyboard Focus To The Window
		ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

		if (!InitGL())									// Initialize Our Newly Created GL Window
		{
			KillGLWindow();								// Reset The Display
			MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
			return FALSE;								// Return FALSE
		}

		return TRUE;									// Success
	}



	void CreateAndStartWorldUpdateThread()
	{
		//Start game world update thread
			
			world_update_thread_handle = CreateThread( NULL, 0, WorldUpdate, &world_update_thread_data, 0, NULL);  
			if ( world_update_thread_handle == NULL)
				ExitProcess(world_update_thread_data);

			thread_handles[0] = world_update_thread_handle;
	}

	void Engine::Start()
	{
		// Create Our OpenGL Window
		if (!CreateGLWindow("NMS Engine test",640,480,16,fullscreen))
		{
			return;
		}
				
		//cout << "Engine started ..." << endl;
		engine_running = true;

		CreateAndStartWorldUpdateThread();

		while(engine_running)
		{
			DrawGLTriangleAndCubeTestScene();					// Draw The Scene
			SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
		}

		//Wait for worker tread to end.
		WaitForMultipleObjects( 1, thread_handles, TRUE, INFINITE);

		//Clean up
		CloseHandle(world_update_thread_handle);

		KillGLWindow();

		//cout << "Engine ended ..." << endl;
	}
}