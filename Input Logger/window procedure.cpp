#include "window procedure.h"

LRESULT CALLBACK windowProcedure(HWND window,UINT message,WPARAM argW,LPARAM argL)
{
	switch(message)
	{
	case WM_KEYDOWN:
		if(argW == VK_ESCAPE)
			PostQuitMessage(0);
		return 0;
	} // end switch
	return DefWindowProcW(window,message,argW,argL);
} // end function windowProcedure