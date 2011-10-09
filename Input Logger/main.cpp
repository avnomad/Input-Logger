#include <iostream>
using std::wcout;
using std::wcin;
using std::wcerr;
using std::wclog;
using std::endl;
using std::left;
using std::right;

#include <windows.h>

int main()
{
	HDC gdiContext;
	if(gdiContext = CreateDCW(L"DISPLAY",nullptr,nullptr,nullptr))	// covers all monitors?
		wclog << L"Screen device context opened succesfully.\n";
	else
		wcerr << L"Could not open device context for the screen.\n";

	if(DeleteDC(gdiContext))	// fails if argument is nullptr
		wclog << L"Screen device context closed succesfully.\n";
	else
		wcerr << L"Could not close the device context for the screen.\n";
	system("pause");
	return 0;
} // end function main
