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

#ifndef PACKET_H
#define PACKET_H

#include <WinTabLoad.h>
#include <ostream>

extern const WTPKT packetContentFlags;

struct Packet
{
	HCTX context;
	UINT status;
	LONG time;
	WTPKT changed;
	UINT serialNumber;
	UINT cursor;
	DWORD buttons;
	DWORD x;
	DWORD y;
	UINT np;
}; // end struct Packet

std::wostream &operator<<(std::wostream &fout, const Packet &packet);

#endif // PACKET_H