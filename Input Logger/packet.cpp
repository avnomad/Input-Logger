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

#include "packet.h"

#include <sstream>
using std::wostringstream;

const WTPKT packetContentFlags = PK_CONTEXT|PK_STATUS|PK_TIME|PK_CHANGED|PK_SERIAL_NUMBER|PK_CURSOR|PK_BUTTONS|PK_X|PK_Y|PK_NORMAL_PRESSURE;

std::wostream &operator<<(std::wostream &fout, const Packet &packet)
{
	wostringstream sout;

	sout << std::boolalpha << L'{';
	sout << L"\"context\":"         << (ULONGLONG)packet.context << L',';
	sout << L"\"status\":"          << packet.status             << L',';
	sout << L"\"time\":"            << packet.time               << L',';
	sout << L"\"changed\":"         << packet.changed            << L',';
	sout << L"\"serial number\":"   << packet.serialNumber       << L',';
	sout << L"\"cursor\":"          << packet.cursor             << L',';
	sout << L"\"buttons\":"         << packet.buttons            << L',';
	sout << L"\"x\":"               << packet.x                  << L',';
	sout << L"\"y\":"               << packet.y                  << L',';
	sout << L"\"normal pressure\":" << packet.np;
	sout << L'}';

	fout << sout.str();
	return fout;
} // end function operator<<
