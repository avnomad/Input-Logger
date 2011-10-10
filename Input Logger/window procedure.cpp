#include "window procedure.h"
#include <WinTabLoad.h>
#include "process input.h"

LRESULT CALLBACK windowProcedure(HWND window,UINT message,WPARAM argW,LPARAM argL)
{
	switch(message)
	{
	case WM_ACTIVATE:
		if((argW & 0xffff) == WA_INACTIVE)
			moveContextToBottom();
		else
			moveContextToTop();
		return 0;
	case WM_KEYDOWN:
		if(argW == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;
	} // end switch
	return DefWindowProcW(window,message,argW,argL);
} // end function windowProcedure