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

#include "process input.h"
#include <WinTabLoad.h>

#include <stdexcept>
using std::runtime_error;

#include <vector>
using std::vector;

#include <algorithm>
using std::for_each;
using std::begin;
using std::end;

#include <fstream>
using std::wofstream;

#include "shared.h"
#include "packet.h"

struct Device
{
	HCTX wtContext;
	UINT queueSize;
	AXIS x;
	AXIS y;
	AXIS np;
}; // end struct Device

static vector<Device> wtDevices;
static wofstream fout;

void initializeInput(HWND window)
{
	LOGCONTEXTW wtLogContext;
	UINT nDevices;
	WORD wintabVersion;

	// start logging
	fout.open(L"log.txt");
	if(!fout)
		throw runtime_error("Could not open file log.txt for writing!");

	fout << L"{\"wintab input packets\":[\n";

	// initialize wintab
	wtLoad();
	if(!WTInfoW(0,0,nullptr))
		throw runtime_error("No information available from WTInfoW.");
	WTInfoW(WTI_INTERFACE,IFC_SPECVERSION,&wintabVersion);
	if(wintabVersion < 0x0101)
		throw runtime_error("Requires WinTab version 1.1 or above.");

	WTInfoW(WTI_INTERFACE,IFC_NDEVICES,&nDevices);
	wtDevices.resize(nDevices);
	for(UINT c = 0 ; c < nDevices ; ++c)
	{
		// open context
		WTInfoW(WTI_DDCTXS+c,0,&wtLogContext);
		wtLogContext.lcOptions = CXO_CSRMESSAGES;
		wtLogContext.lcLocks = CXL_INSIZE|CXL_MARGIN|CXL_SENSITIVITY|CXL_SYSOUT;
		wtLogContext.lcDevice = c;
		wtLogContext.lcPktRate = 200;
		wtLogContext.lcPktData = packetContentFlags;
		wtLogContext.lcPktMode = 0;
		wtLogContext.lcMoveMask = ~0;
		wtLogContext.lcBtnDnMask = wtLogContext.lcBtnUpMask = ~0;
		WTInfoW(WTI_DEVICES+c,DVC_X,&wtDevices[c].x);
		wtLogContext.lcInOrgX = wtLogContext.lcOutOrgX = wtDevices[c].x.axMin;
		wtLogContext.lcInExtX = wtLogContext.lcOutExtX = wtDevices[c].x.axMax-wtDevices[c].x.axMin+1;
		WTInfoW(WTI_DEVICES+c,DVC_Y,&wtDevices[c].y);
		wtLogContext.lcInOrgY = wtLogContext.lcOutOrgY = wtDevices[c].y.axMin;
		wtLogContext.lcInExtY = wtLogContext.lcOutExtY = wtDevices[c].y.axMax-wtDevices[c].y.axMin+1;
		WTInfoW(WTI_DEVICES+c,DVC_NPRESSURE,&wtDevices[c].np);
		if((wtDevices[c].wtContext = WTOpenW(window,&wtLogContext,FALSE)) == nullptr)
			throw runtime_error("Could not open desired context(s).");
		WTGetW(wtDevices[c].wtContext,&wtLogContext);	// to see what field were changed by WTOpen during debugging.

		// configure context queue size
		wtDevices[c].queueSize = WTQueueSizeGet(wtDevices[c].wtContext);
		if(wtDevices[c].queueSize < 10)
			if(!WTQueueSizeSet(wtDevices[c].wtContext,10))
				if(!WTQueueSizeSet(wtDevices[c].wtContext,wtDevices[c].queueSize))
					throw runtime_error("Could not restore WinTab context queue size.");	// can think of better ways to respond...
		wtDevices[c].queueSize = WTQueueSizeGet(wtDevices[c].wtContext);

		// start receiving input
		WTEnable(wtDevices[c].wtContext,TRUE);
		WTOverlap(wtDevices[c].wtContext,TRUE);
	} // end for
} // end function initializeInput


void processInput(void)
{
	vector<Packet> packets;
	for_each(begin(wtDevices),end(wtDevices),[&packets](const Device &device)
	{
		packets.resize(device.queueSize);
		int size = WTPacketsGet(device.wtContext,packets.size(),packets.data());
		packets.resize(size);
		for_each(begin(packets),end(packets),[&device](const Packet &packet)
		{
			// normally when we ask for the packets of a context we should get 
			// the packets of that context, but this doesn't seem to be the case 
			// with Wacom's Cintiq Wintab 64-bit implementation...
			// so has to work around this bug.
			if(device.wtContext != packet.context)	// if packet is not from out context...
			{
				fout << "\"wrong context!\",\n";
				return;	// ...ignore the packet
			} // end if
			fout << packet << L",\n";	// log packet

			DWORD type;
			WTInfoW(WTI_CURSORS+packet.cursor,CSR_TYPE,&type);
			if(packet.np > 10)
			{
				Point p;
				p.position.x = double(packet.x - device.x.axMin)/(device.x.axMax-device.x.axMin);
				p.position.y = double(packet.y - device.y.axMin)/(device.y.axMax-device.y.axMin);
				p.pressure = double(packet.np - device.np.axMin)/(device.np.axMax-device.np.axMin);
				currentStrokes[(packet.cursor+type)].push_back(p);
			} // end if
			if(packet.np <= 10 && currentStrokes.count((packet.cursor+type)))
			{
				completeStrokes.push_back(*currentStrokes.find((packet.cursor+type)));
				currentStrokes.erase((packet.cursor+type));
			} // end if
		}); // end for_each
	}); // end for_each
} // end function processInput


void cleanUpInput()	// bad design! WTClose should be called before window is destroyed. wtUnLoad
{					// should be called only once
	for_each(begin(wtDevices),end(wtDevices),[](const Device &device)
	{
		WTClose(device.wtContext);
	}); // end for_each
	wtDevices.clear();

	wtUnLoad();

	// finish logging
	fout << L"null]}";
	fout.close();
} // end function cleanUpInput


void moveContextToTop()
{
	for_each(begin(wtDevices),end(wtDevices),[](const Device &device)
	{
		WTOverlap(device.wtContext,TRUE);
	}); // end for_each
} // end function moveContextToTop


void moveContextToBottom()
{
	for_each(begin(wtDevices),end(wtDevices),[](const Device &device)
	{
		WTOverlap(device.wtContext,FALSE);
	}); // end for_each
} // end function moveContextToBottom
