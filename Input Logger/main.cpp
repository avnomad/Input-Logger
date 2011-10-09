#include <iostream>
using std::wcout;
using std::wcin;
using std::wcerr;
using std::wclog;
using std::endl;
using std::left;
using std::right;

#include <windows.h>

#include <gl/glew.h>
//#include <gl/glut.h>

int main()
{
	HDC gdiContext;
	HGLRC glContext;
	PIXELFORMATDESCRIPTOR pixelFormatDescription = {0};
	int pixelFormatIndex;
	int width;
	int height;
	
	// CreateDC
	//if(gdiContext = CreateDCW(L"DISPLAY",nullptr,nullptr,nullptr))	// covers all monitors?
	//	wclog << L"Screen device context opened succesfully.\n";
	//else
	//	wcerr << L"Could not open device context for the screen.\n";
	
	if(gdiContext = GetDC(GetDesktopWindow()))	// covers all monitors?
		wclog << L"Desktop device context opened succesfully.\n";
	else
		wcerr << L"Could not open device context for the desktop.\n";

	width = GetDeviceCaps(gdiContext,HORZRES);
	height = GetDeviceCaps(gdiContext,VERTRES);

	// create OpenGL rendering context
	pixelFormatDescription.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormatDescription.nVersion = 1;
	pixelFormatDescription.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER|PFD_STEREO_DONTCARE;
	pixelFormatDescription.iPixelType = PFD_TYPE_RGBA;
	pixelFormatDescription.cColorBits = 64;
	pixelFormatDescription.cAlphaBits = 16;
	pixelFormatDescription.cAccumBits = 128;
	pixelFormatDescription.cDepthBits = 64;
	pixelFormatDescription.cStencilBits = 32;
	pixelFormatDescription.cAuxBuffers = 128;
	pixelFormatDescription.iLayerType = PFD_MAIN_PLANE;

	if(pixelFormatIndex = ChoosePixelFormat(gdiContext,&pixelFormatDescription))
		wclog << L"Pixel format selected succesfully.\n";
	else
		wcerr << L"Could not select pixel format.\n";
	if(SetPixelFormat(gdiContext,pixelFormatIndex,&pixelFormatDescription))
		wclog << L"Pixel format set succesfully.\n";
	else
		wcerr << L"Could not set pixel format.\n";
	if(glContext = wglCreateContext(gdiContext))
		wclog << L"OpenGL rendering context created succesfully.\n";
	else
		wcerr << L"Could not create OpenGL rendering context .\n";
	if(wglMakeCurrent(gdiContext,glContext))
		wclog << L"Selected current context succesfully.\n";
	else
		wcerr << L"Could not select current context .\n";

	// initialize glew
	glewInit();

	// initialize OpenGL
	glClearColor(1.0,0.5,0.0,0.0);

	//while(true)
	//{
	//	glClear(GL_COLOR_BUFFER_BIT);
	//	SwapBuffers(gdiContext);
	//} // end while
	wglMakeCurrent(nullptr,nullptr);

	if(wglDeleteContext(glContext))
		wclog << L"OpenGL rendering context deleted succesfully.\n";
	else
		wcerr << L"Could not delete OpenGL rendering context .\n";

	if(ReleaseDC(GetDesktopWindow(),gdiContext))	// fails if argument is nullptr
		wclog << L"Desktop device context closed succesfully.\n";
	else
		wcerr << L"Could not close the device context for the desktop.\n";

	// DeleteDC
	//if(DeleteDC(gdiContext))	// fails if argument is nullptr
	//	wclog << L"Screen device context closed succesfully.\n";
	//else
	//	wcerr << L"Could not close the device context for the screen.\n";
	system("pause"); 
	return 0; 
} // end function main
