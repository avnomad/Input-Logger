//	Copyright (C) 2011-2012 Vaptistis Anogeianakis <nomad@cornercase.gr>
/*
 *	This file is part of Input Logger.
 *
 *	Input Logger is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Input Logger is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Input Logger.  If not, see <http://www.gnu.org/licenses/>.
 */

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
	window = CreateWindowExW(0,L"fullScreenWindow",L"WinTab input logger",WS_POPUP|WS_CLIPCHILDREN|WS_CLIPSIBLINGS,0,0,	// size of
					GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);	// primary display
	ShowWindow(window,SW_SHOWNORMAL);

	// get device context handle
	gdiContext = GetDC(window);

	// create OpenGL rendering context
	pixelFormatDescription.nSize        = sizeof(PIXELFORMATDESCRIPTOR);
	pixelFormatDescription.nVersion     = 1;
	pixelFormatDescription.dwFlags      = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER|PFD_STEREO_DONTCARE;
	pixelFormatDescription.iPixelType   = PFD_TYPE_RGBA;
	pixelFormatDescription.cColorBits   = 64;
	pixelFormatDescription.cAlphaBits   = 16;
	pixelFormatDescription.cAccumBits   = 128;
	pixelFormatDescription.cDepthBits   = 64;
	pixelFormatDescription.cStencilBits = 32;
	pixelFormatDescription.cAuxBuffers  = 128;
	pixelFormatDescription.iLayerType   = PFD_MAIN_PLANE;
	
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

	// misc
	ShowCursor(FALSE);

	// initialize OpenGL
	glClearColor(0.0,0.0,0.0,0.0);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0,1,0,1,0,1);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);

	// main loop
	do
	{
		while(PeekMessageW(&message,nullptr,0,0,PM_REMOVE))
		{
			if(message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessageW(&message);
		} // end while

		RECT r;
		processInput();
		GetClientRect(window,&r);
		renderFrame(r.right,r.bottom);

		SwapBuffers(gdiContext);
	} // end while
	while(message.message != WM_QUIT);

	// clean up
	wglMakeCurrent(nullptr,nullptr);
	if(wglDeleteContext(glContext))
		wclog << L"OpenGL rendering context deleted succesfully.\n";
	else
		wcerr << L"Could not delete OpenGL rendering context .\n";
	UnregisterClassW(L"fullScreenWindow",GetModuleHandleW(nullptr));	// not necessary

	//system("pause"); 
	return 0; 
} // end function main
