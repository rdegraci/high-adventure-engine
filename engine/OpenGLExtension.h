/*
    High Adventure 3D Game Engine
    Copyright (C) 2002  Rodney Degracia

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

	rodney_degracia(at)elitefrontier.com

	Rodney Degracia
	316 Independence Ave. SE 
	Washington DC 2003
*/


//#include <GL/gl.h>
#include "glext.h"

class OpenGLExtension
{

	GLboolean QueryExtension(char *extName)
	{
		char *p = (char *) glGetString(GL_EXTENSIONS);
		char *end = p + strlen(p);
		while (p < end) {
		size_t n = strcspn(p, " ");
		if ((strlen(extName)==n) && (strncmp(extName,p,n)==0)) {
		return GL_TRUE;
		}
		p += (n + 1);
		}
		return GL_FALSE;
	}

public:
	
	static void (APIENTRY* g_pglDrawRangeElements) (GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid*);

	void Load()
	{
		assert(QueryExtension("GL_EXT_draw_range_elements"));
		g_pglDrawRangeElements=(void (APIENTRY*) (GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid*))wglGetProcAddress("glDrawRangeElements");	
		
		assert(g_pglDrawRangeElements);
	}
};

void (APIENTRY* OpenGLExtension::g_pglDrawRangeElements) (GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid*)=0;