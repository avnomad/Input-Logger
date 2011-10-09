#include "render frame.h"
#include <gl/glew.h>

void renderFrame(int width,int height)
{
	glViewport(0,0,width,height);
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_LINES);
		glVertex2d(-1.0,1.0);
		glVertex2d(1.0,-1.0);
		glVertex2d(-1.0,-1.0);
		glVertex2d(1.0,1.0);
	glEnd();
} // end function renderFrame
