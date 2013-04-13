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

#include "render frame.h"
#include <gl/glew.h>

#include "shared.h"

#include <algorithm>
using std::for_each;
using std::begin;
using std::end;

//#include <array>
//using std::array;
#define length(A) (sizeof(A)/sizeof((A)[0]))

float colors[][3] = {
	{0.5,0.25,0},	// brown
	{0,1,0},		// lime
	{0,0.5,0},		// green
	{1,1,0},		// yellow
	{1,0,1},		// purple
	{1,0,0},		// red
	{1,0.5,0},		// orange
	{1,0.75,0},		// gold
	{0,0.25,1},		// azure
	{0,1,1},		// cyan
	{1,1,1},		// white
	{0,0,1},		// blue
};


void renderFrame(int width,int height)
{
	glViewport(0,0,width,height);
	glClear(GL_COLOR_BUFFER_BIT);

	for_each(begin(currentStrokes),end(currentStrokes),[](const pair<unsigned int,vector<Point>> &pair)
	{
		float *color = colors[pair.first % length(colors)];
		glBegin(GL_LINE_STRIP);
			for_each(begin(pair.second),end(pair.second),[color](const Point &point)
			{
				glColor4f(color[0],color[1],color[2],point.pressure);
				glVertex2dv(point.position);
			}); // end for_each
		glEnd();
	}); // end for_each

	for_each(begin(completeStrokes),end(completeStrokes),[](const pair<unsigned int,vector<Point>> &pair)
	{
		float *color = colors[pair.first % length(colors)];
		glBegin(GL_LINE_STRIP);
			for_each(begin(pair.second),end(pair.second),[color](const Point &point)
			{
				glColor4f(color[0],color[1],color[2],point.pressure);
				glVertex2dv(point.position);
			}); // end for_each
		glEnd();
	}); // end for_each

} // end function renderFrame
