//	Copyright (C) 2011-2012 Vaptistis Anogeianakis <el05208@mail.ntua.gr>
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

#include "window procedure.h"
#include <WindowsX.h>

#include "process input.h"
#include "shared.h"
#define BASE 9998

LRESULT CALLBACK windowProcedure(HWND window,UINT message,WPARAM argW,LPARAM argL)
{
	Point p;
	static unsigned int index;

	switch(message)
	{
	case WM_CREATE:
		initializeInput(window);
		return 0;
	case WM_LBUTTONDOWN:	// normally I should make sure the capture is released by
	case WM_MBUTTONDOWN:	// the same button that captured it... etc.
	case WM_RBUTTONDOWN:
		if(GetCapture() != window)
		{
			SetCapture(window);
			index = BASE+argW;

			p.position.x = GET_X_LPARAM(argL)/(double)GetSystemMetrics(SM_CXSCREEN);
			p.position.y = (GetSystemMetrics(SM_CYSCREEN)-GET_Y_LPARAM(argL))/(double)GetSystemMetrics(SM_CYSCREEN);
			p.pressure   = 0.75;
			currentStrokes[index].push_back(p);
		} // end if
		return 0;
	case WM_MOUSEMOVE:
		if(GetCapture() == window)
		{
			p.position.x = GET_X_LPARAM(argL)/(double)GetSystemMetrics(SM_CXSCREEN);
			p.position.y = (GetSystemMetrics(SM_CYSCREEN)-GET_Y_LPARAM(argL))/(double)GetSystemMetrics(SM_CYSCREEN);
			p.pressure   = 0.75;
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
