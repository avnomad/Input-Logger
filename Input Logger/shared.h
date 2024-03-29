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

#ifndef SHARED_H
#define SHARED_H

#include <vector>
using std::vector;

#include <map>
using std::map;

#include <utility>
using std::pair;

#include <Space-Time/Vector2D.h>

struct Point
{
	Vector2D<> position;
	double pressure;
};

extern vector<pair<unsigned int,vector<Point>>> completeStrokes;
extern map<unsigned int, vector<Point>> currentStrokes;

#endif // SHARED_H
