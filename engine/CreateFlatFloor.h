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

class CreateFlatFloor
{
public:
	void Draw_XYZ_Axis()
	{
		glPushMatrix();
		glColor3f(1,0,0);
		glBegin(GL_LINES);
		glVertex3f(-25.0f,0.0f,0.0f);
		glVertex3f(25.0f,0.0f,0.0f);
		glEnd();

		
		glBegin(GL_LINES);
		glVertex3f(0.0f,-25.0f,0.0f);
		glVertex3f(0.0f,25.0f,0.0f);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(0.0f,0.0f,-25.0f);
		glVertex3f(0.0f,0.0f,25.0f);
		glEnd();
		glPopMatrix();
	}

	void Draw_Triangle()
	{
		glBegin(GL_TRIANGLES);
		glVertex3f(0.0f,0.0f,0.0f);
		glVertex3f(1.0f,0.0f,0.0f);
		glVertex3f(1.0f,1.0f,0.0f);
		glEnd();
	}

	void DrawFloor()  
	{
		glPushMatrix();
		glPushAttrib( GL_LIGHTING_BIT );
		glDisable( GL_LIGHTING );
			glColor3f( 0.0, 1.0f, 0.0  );
			glBegin( GL_LINES );
				GLfloat zExtent( 25600 );
				for( int loopX = -25600; loopX <= 25600; loopX+=64 )
				{
					GLfloat xLocal( static_cast<float>(loopX) );
					glVertex3f( xLocal, 0.0, -zExtent );
					glVertex3f( xLocal, 0.0,  zExtent );
				}
				GLfloat xExtent( 25600 );
				for( int loopZ = -25600; loopZ <= 25600; loopZ+=64 )
				{
					GLfloat zLocal( static_cast<float>(loopZ) );
					glVertex3f( -xExtent, 0.0, zLocal );
					glVertex3f(  xExtent, 0.0, zLocal );
				}
			glEnd();
		glPopAttrib();
		glPopMatrix();
		glColor3f( 1.0f, 1.0f, 1.0f );
	}

	void Init()
	{
	}

	void Render()
	{
		Draw_XYZ_Axis();
		DrawFloor();
		glFlush();

		//Clear the color to white so that
		//any textures will be modulated correctly
		glColor3f( 1.0f, 1.0f, 1.0f );
	}
};