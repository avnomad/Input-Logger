#include <iostream>
using std::wcout;
using std::wcin;
using std::wcerr;
using std::wclog;
using std::endl;
using std::left;
using std::right;

#include <gl/glew.h>

#include "window procedure.h"
#include "process input.h"
#include "render frame.h"

int main()
{
	WNDCLASSEXW wc = {sizeof(WNDCLASSEXW),0};
	HWND window;
	MSG message;
	HDC gdiContext;
	HGLRC glContext;
	PIXELFORMATDESCRIPTOR pixelFormatDescription = {0};
	int pixelFormatIndex;

	// register a window class
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = windowProcedure;
	wc.hInstance = GetModuleHandleW(nullptr);
	wc.lpszClassName = L"fullScreenWindow";
	RegisterClassExW(&wc);

	// create a window
	window = CreateWindowExW(0,L"fullScreenWindow",L"WinTab input logger",WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE,0,0,	// size of
					GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);	// primary display

	// hide cursor
	ShowCursor(FALSE);

	// get device context handle
	gdiContext = GetDC(window);

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

	// main loop
	while(GetMessageW(&message,nullptr,0,0))
	{
		RECT r;
		TranslateMessage(&message);
		DispatchMessageW(&message);

		processInput();
		GetClientRect(window,&r);
		renderFrame(r.right,r.bottom);

		SwapBuffers(gdiContext);
	} // end while

	// clean up
	wglMakeCurrent(nullptr,nullptr);
	if(wglDeleteContext(glContext))
		wclog << L"OpenGL rendering context deleted succesfully.\n";
	else
		wcerr << L"Could not delete OpenGL rendering context .\n";
	DestroyWindow(window);
	UnregisterClassW(L"fullScreenWindow",GetModuleHandleW(nullptr));	// not necessary

	//system("pause"); 
	return 0; 
} // end function main