#include "process input.h"
#include <WinTabLoad.h>

#include <stdexcept>
using std::runtime_error;

#include <vector>
using std::vector;

static vector<HCTX> wtContexts;

void initializeInput(void)
{
	wtLoad();
	UINT nDevices;
	if(!WTInfoW(0,0,nullptr))
		throw runtime_error("No information available from WTInfoW");

	WTInfoW(WTI_INTERFACE,IFC_NDEVICES,&nDevices);
	for(UINT c = 0 ; c < nDevices ; ++c)
	{


	} // end for
} // end function initializeInput


void processInput(void)
{

} // end function processInput


void cleanUpInput()
{
	wtUnLoad();
} // end function cleanUpInput