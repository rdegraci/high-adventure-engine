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

#pragma once

#include <SDL.h>
#include <SDL_opengl.h>	

template <typename T,
		  typename OpenGLViewFrustumTrait>
class OpenGLViewFrustum
{
protected:
	
	typename OpenGLViewFrustumTrait::ModelViewMatrix m_ModelViewMatrix;
	typename OpenGLViewFrustumTrait::ProjectionMatrix m_ProjectionMatrix;
	typename OpenGLViewFrustumTrait::ViewPlaneMatrix m_ViewPlaneMatrix;	//6x4
	typename OpenGLViewFrustumTrait::ViewPlaneMatrix m_Save;	//Temporary Storage

	~OpenGLViewFrustum()
	{
	}

	//TODO: Consider Templating
	void GetModelViewFromOpenGL() 
	{
		float M[16];
		glGetFloatv(GL_MODELVIEW_MATRIX,M);

		m_ModelViewMatrix.m_Storage.M[0][0]=M[0],m_ModelViewMatrix.m_Storage.M[0][1]=M[4],m_ModelViewMatrix.m_Storage.M[0][2]=M[8],m_ModelViewMatrix.m_Storage.M[0][3]=M[12];
		m_ModelViewMatrix.m_Storage.M[1][0]=M[1],m_ModelViewMatrix.m_Storage.M[1][1]=M[5],m_ModelViewMatrix.m_Storage.M[1][2]=M[9],m_ModelViewMatrix.m_Storage.M[1][3]=M[13];
		m_ModelViewMatrix.m_Storage.M[2][0]=M[2],m_ModelViewMatrix.m_Storage.M[2][1]=M[6],m_ModelViewMatrix.m_Storage.M[2][2]=M[10],m_ModelViewMatrix.m_Storage.M[2][3]=M[14];
		m_ModelViewMatrix.m_Storage.M[3][0]=M[3],m_ModelViewMatrix.m_Storage.M[3][1]=M[7],m_ModelViewMatrix.m_Storage.M[3][2]=M[11],m_ModelViewMatrix.m_Storage.M[3][3]=M[15];

	}
	
	//TODO: Consider Templating
	void GetProjectMatrixFromOpenGL() 
	{
		float M[16];
		glGetFloatv(GL_PROJECTION_MATRIX,M);

		m_ProjectionMatrix.m_Storage.M[0][0]=M[0],m_ProjectionMatrix.m_Storage.M[0][1]=M[4],m_ProjectionMatrix.m_Storage.M[0][2]=M[8],m_ProjectionMatrix.m_Storage.M[0][3]=M[12];
		m_ProjectionMatrix.m_Storage.M[1][0]=M[1],m_ProjectionMatrix.m_Storage.M[1][1]=M[5],m_ProjectionMatrix.m_Storage.M[1][2]=M[9],m_ProjectionMatrix.m_Storage.M[1][3]=M[13];
		m_ProjectionMatrix.m_Storage.M[2][0]=M[2],m_ProjectionMatrix.m_Storage.M[2][1]=M[6],m_ProjectionMatrix.m_Storage.M[2][2]=M[10],m_ProjectionMatrix.m_Storage.M[2][3]=M[14];
		m_ProjectionMatrix.m_Storage.M[3][0]=M[3],m_ProjectionMatrix.m_Storage.M[3][1]=M[7],m_ProjectionMatrix.m_Storage.M[3][2]=M[11],m_ProjectionMatrix.m_Storage.M[3][3]=M[15];

	}

public:

	typedef typename OpenGLViewFrustumTrait::Matrix4x4 Matrix;

	inline void SaveState()
	{
		m_Save=m_ViewPlaneMatrix;
	}

	inline void RestoreState()
	{
		m_ViewPlaneMatrix=m_Save;
	}



	inline typename OpenGLViewFrustumTrait::Plane GetRightPlane() const
	{
	   return (typename OpenGLViewFrustumTrait::Plane(	m_ViewPlaneMatrix.m_Storage.M[0][0],
								m_ViewPlaneMatrix.m_Storage.M[0][1],
								m_ViewPlaneMatrix.m_Storage.M[0][2],
								m_ViewPlaneMatrix.m_Storage.M[0][3]
								));
	}

	inline typename OpenGLViewFrustumTrait::Plane GetLeft() const
	{
		return (typename OpenGLViewFrustumTrait::Plane(	m_ViewPlaneMatrix.m_Storage.M[1][0],
								m_ViewPlaneMatrix.m_Storage.M[1][1],
								m_ViewPlaneMatrix.m_Storage.M[1][2],
								m_ViewPlaneMatrix.m_Storage.M[1][3]
								));
	}


	inline typename OpenGLViewFrustumTrait::Plane GetTop() const
	{
		return (typename OpenGLViewFrustumTrait::Plane(	m_ViewPlaneMatrix.m_Storage.M[2][0],
								m_ViewPlaneMatrix.m_Storage.M[2][1],
								m_ViewPlaneMatrix.m_Storage.M[2][2],
								m_ViewPlaneMatrix.m_Storage.M[2][3]
								));
	}

	inline typename OpenGLViewFrustumTrait::Plane GetBottom() const
	{
		return (typename OpenGLViewFrustumTrait::Plane(	m_ViewPlaneMatrix.m_Storage.M[3][0],
								m_ViewPlaneMatrix.m_Storage.M[3][1],
								m_ViewPlaneMatrix.m_Storage.M[3][2],
								m_ViewPlaneMatrix.m_Storage.M[3][3]
								));
	}


	inline typename OpenGLViewFrustumTrait::Plane GetNear() const
	{
		return (OpenGLViewFrustumTrait::Plane(	m_ViewPlaneMatrix.m_Storage.M[4][0],
								m_ViewPlaneMatrix.m_Storage.M[4][1],
								m_ViewPlaneMatrix.m_Storage.M[4][2],
								m_ViewPlaneMatrix.m_Storage.M[4][3]
								));
	}


	inline typename OpenGLViewFrustumTrait::Plane GetFar() const
	{
		return (typename OpenGLViewFrustumTrait::Plane(	m_ViewPlaneMatrix.m_Storage.M[5][0],
								m_ViewPlaneMatrix.m_Storage.M[5][1],
								m_ViewPlaneMatrix.m_Storage.M[5][2],
								m_ViewPlaneMatrix.m_Storage.M[5][3]
								));
	}

	inline void ExtractPlanes()
	{
		GetModelViewFromOpenGL();
		GetProjectMatrixFromOpenGL();

		//Matrix should be treated as column major,
		//since OpenGL treats matrices similarly.
		typename OpenGLViewFrustumTrait::Matrix4x4 m=m_ProjectionMatrix*m_ModelViewMatrix;

		/*	The frustum planes
         [ -1  0  0  1 ]	Right
         [  1  0  0  1 ]	Left
         [  0 -1  0  1 ]	Top
         [  0  1  0  1 ]	Bottom
         [  0  0 -1  1 ]	Far		//OpenGL	
         [  0  0  1  1 ]	Near	//OpenGL
		*/

		/* OpenGL Column Major

			A		B	   C		D
         [ m3-m0  m7-m4  m11-m8  m15-m12 ]	Right
         [ m3+m0  m7+m4  m11+m8  m15+m12 ]	Left
         [ m3-m1  m7-m5  m11-m9  m15-m13 ]	Top
         [ m3+m1  m7+m5  m11+m9  m15+m13 ]	Bottom
         [ m3-m2  m7-m6  m11-m10 m15-m14 ]	Far		//OpenGL
         [ m3+m2  m7+m6  m11+m10 m15+m14 ]	Near	//OpenGL
		*/

		//Quick and dirty transform of the initial 6 planes of the viewfrustum

		//Right
		m_ViewPlaneMatrix.m_Storage.M[0][0]=(m.m_Storage.M[3][0]-m.m_Storage.M[0][0]);
		m_ViewPlaneMatrix.m_Storage.M[0][1]=(m.m_Storage.M[3][1]-m.m_Storage.M[0][1]);
		m_ViewPlaneMatrix.m_Storage.M[0][2]=(m.m_Storage.M[3][2]-m.m_Storage.M[0][2]);
		m_ViewPlaneMatrix.m_Storage.M[0][3]=(m.m_Storage.M[3][3]-m.m_Storage.M[0][3]);

		//Left
		m_ViewPlaneMatrix.m_Storage.M[1][0]=(m.m_Storage.M[3][0]+m.m_Storage.M[0][0]);
		m_ViewPlaneMatrix.m_Storage.M[1][1]=(m.m_Storage.M[3][1]+m.m_Storage.M[0][1]);
		m_ViewPlaneMatrix.m_Storage.M[1][2]=(m.m_Storage.M[3][2]+m.m_Storage.M[0][2]);
		m_ViewPlaneMatrix.m_Storage.M[1][3]=(m.m_Storage.M[3][3]+m.m_Storage.M[0][3]);

		//Top
		m_ViewPlaneMatrix.m_Storage.M[2][0]=(m.m_Storage.M[3][0]-m.m_Storage.M[1][0]);
		m_ViewPlaneMatrix.m_Storage.M[2][1]=(m.m_Storage.M[3][1]-m.m_Storage.M[1][1]);
		m_ViewPlaneMatrix.m_Storage.M[2][2]=(m.m_Storage.M[3][2]-m.m_Storage.M[1][2]);
		m_ViewPlaneMatrix.m_Storage.M[2][3]=(m.m_Storage.M[3][3]-m.m_Storage.M[1][3]);

		//Bottom
		m_ViewPlaneMatrix.m_Storage.M[3][0]=(m.m_Storage.M[3][0]+m.m_Storage.M[1][0]);
		m_ViewPlaneMatrix.m_Storage.M[3][1]=(m.m_Storage.M[3][1]+m.m_Storage.M[1][1]);
		m_ViewPlaneMatrix.m_Storage.M[3][2]=(m.m_Storage.M[3][2]+m.m_Storage.M[1][2]);
		m_ViewPlaneMatrix.m_Storage.M[3][3]=(m.m_Storage.M[3][3]+m.m_Storage.M[1][3]);

		//Near
		m_ViewPlaneMatrix.m_Storage.M[4][0]=(m.m_Storage.M[3][0]+m.m_Storage.M[2][0]);
		m_ViewPlaneMatrix.m_Storage.M[4][1]=(m.m_Storage.M[3][1]+m.m_Storage.M[2][1]);
		m_ViewPlaneMatrix.m_Storage.M[4][2]=(m.m_Storage.M[3][2]+m.m_Storage.M[2][2]);
		m_ViewPlaneMatrix.m_Storage.M[4][3]=(m.m_Storage.M[3][3]+m.m_Storage.M[2][3]);

		//Far
		m_ViewPlaneMatrix.m_Storage.M[5][0]=(m.m_Storage.M[3][0]-m.m_Storage.M[2][0]);
		m_ViewPlaneMatrix.m_Storage.M[5][1]=(m.m_Storage.M[3][1]-m.m_Storage.M[2][1]);
		m_ViewPlaneMatrix.m_Storage.M[5][2]=(m.m_Storage.M[3][2]-m.m_Storage.M[2][2]);
		m_ViewPlaneMatrix.m_Storage.M[5][3]=(m.m_Storage.M[3][3]-m.m_Storage.M[2][3]);



	}
};