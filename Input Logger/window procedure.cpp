#include "window procedure.h"
#include <WindowsX.h>

#include "process input.h"
#include "shared.h"
#define BASE 10004

LRESULT CALLBACK windowProcedure(HWND window,UINT message,WPARAM argW,LPARAM argL)
{
	Point p;
	static unsigned int index;

	switch(message)
	{
	case WM_CREATE:
		initializeInput(window);
		return 0;
	case WM_LBUTTONDOWN:	// normally I should make sure the capture is release by
	case WM_MBUTTONDOWN:	// the same button that captures it... etc.
	case WM_RBUTTONDOWN:
		if(GetCapture() != window)
		{
			SetCapture(window);
			index = BASE+argW;

			p.position.x = GET_X_LPARAM(argL)/(double)GetSystemMetrics(SM_CXSCREEN);
			p.position.y = (GetSystemMetrics(SM_CYSCREEN)-GET_Y_LPARAM(argL))/(double)GetSystemMetrics(SM_CYSCREEN);
			p.pressure = 1.0;
			currentStrokes[index].push_back(p);
		} // end if
		return 0;
	case WM_MOUSEMOVE:
		if(GetCapture() == window)
		{
			p.position.x = GET_X_LPARAM(argL)/(double)GetSystemMetrics(SM_CXSCREEN);
			p.position.y = (GetSystemMetrics(SM_CYSCREEN)-GET_Y_LPARAM(argL))/(double)GetSystemMetrics(SM_CYSCREEN);
			p.pressure = 1.0;
			currentStrokes[index].push_back(p);
		} // end if
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		if(GetCapture() == window)
		{
			completeStrokes.push_back(*currentStrokes.find(index));
			currentStrokes.erase(index);
			ReleaseCapture();
		} // end if
		return 0;
	case WM_ACTIVATE:
		if((argW & 0xffff) == WA_INACTIVE)
			moveContextToBottom();
		else
			moveContextToTop();
		return 0;
	case WM_KEYDOWN:
		if(argW == VK_ESCAPE)
			DestroyWindow(window);
		return 0;
	case WM_DESTROY:
		cleanUpInput();
		PostQuitMessage(0);
		return 0;
	} // end switch
	return DefWindowProcW(window,message,argW,argL);
} // end function windowProcedure