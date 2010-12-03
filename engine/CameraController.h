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

#include "CameraControllerTrait.h"
#include "KeyboardPolicy.h"

template <typename T,
		  typename CameraControllerTrait,
		  template <typename,typename> class KeyboardPolicy>
class CameraController
{
public:
};


template <typename T,
		  template <typename,typename> class KeyboardPolicy>
class CameraController<T,FirstPersonView,KeyboardPolicy> : public KeyboardPolicy<T,FirstPersonView>
{
public:
	FirstPersonView::Point m_WorldOrigin;
	FirstPersonView::Point m_EyepointOrigin;
	FirstPersonView::CylinderCoords m_CCS;

	T m_EyepointRotation;	//Relative to m_WorldRotation
	T m_WorldRotation;		//Relative to m_EyepointRotation

public:

	CameraController()
	{


		//Setup GL viewport
		glViewport(0,0,800,600);
		glFrustum(-1.0f,1.0f,-1.0f,1.0f,1.5f,4096.0f);	//90 degree FOV
		glClearColor(0,0,0,0);

		m_EyepointRotation=0.0f;
		m_WorldRotation=0.0f;

		m_WorldOrigin.SetZero();
		m_EyepointOrigin.SetZero();
	}



	void SetEyepointPosition(float x, float y, float z)
	{
		m_EyepointOrigin.Set(x,y,z);
		m_WorldOrigin.Set(-x,-y,-z);
	}

	void Clamp()
	{
		//Range [0,360]

		while(m_EyepointRotation < 0.0)
		{
			m_EyepointRotation += 360.0;
		}
		while( m_EyepointRotation >= 360.0)
		{
			m_EyepointRotation -= 360.0;
		}

	}

	void Rotate()
	{
		glRotatef(m_WorldRotation,0,1,0);		

		if(panleft)
		{
			m_EyepointRotation += 4;	
			Clamp();
		}
		else if(panright)
		{
			m_EyepointRotation -= 4;	
			Clamp();
		}

		m_WorldRotation=-m_EyepointRotation;
	}

	void Translate()
	{
		if(dollyin)
		{
			m_CCS.SetAngleDegrees(m_EyepointRotation+0.0f);
			m_CCS.m_R=1.0f;
			Vector3x1<float> v=Cylindrical2Cartesian<float,RealNumberSystem,StandardEuclidean>(m_CCS);
			m_WorldOrigin.x+=v.x*10; 
			m_WorldOrigin.z+=v.y*10;
		}
		else if(dollyout)
		{
			m_CCS.SetAngleDegrees(m_EyepointRotation+180.0f);
			m_CCS.m_R=1.0f;
			Vector3x1<float> v=Cylindrical2Cartesian<float,RealNumberSystem,StandardEuclidean>(m_CCS);
			m_WorldOrigin.x+=v.x*10; 
			m_WorldOrigin.z+=v.y*10;
		}
		else if(trackleft) 
		{		
			m_CCS.SetAngleDegrees(m_EyepointRotation+90.0f);
			m_CCS.m_R=1.0f;
			Vector3x1<float> v=Cylindrical2Cartesian<float,RealNumberSystem,StandardEuclidean>(m_CCS);
			m_WorldOrigin.x+=v.x*10;
			m_WorldOrigin.z+=v.y*10;
		}
		else if(trackright) 
		{
			m_CCS.SetAngleDegrees(m_EyepointRotation+270.0f);
			m_CCS.m_R=1.0f;
			Vector3x1<float> v=Cylindrical2Cartesian<float,RealNumberSystem,StandardEuclidean>(m_CCS);
			m_WorldOrigin.x+=v.x*10; 
			m_WorldOrigin.z+=v.y*10;
		}
	}


	void Click()
	{
		HandleKeyBoardInput();

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		Rotate();
		Translate();

		glTranslatef(m_WorldOrigin.x,m_WorldOrigin.y,m_WorldOrigin.z);
	}
};


