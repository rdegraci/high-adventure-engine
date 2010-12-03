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

#include "RendererReprTrait.h"
#include "FrustumPolicy.h"
#include "CullingPolicy.h"
#include "RenderPolicy.h"

template< typename T,
		  typename RendererReprTrait=HardwareRendering,
		  template <typename, typename> class FrustumPolicy=OpenGLViewFrustum,
		  template <typename, typename> class RenderPolicy=Accelerated,
		  template <typename, typename> class CullingPolicy=BoundingSphereHierarchy>
class RendererRepr : public FrustumPolicy<T,RendererReprTrait>,
					 public RenderPolicy<T,RendererReprTrait>,
					 public CullingPolicy<T,RendererReprTrait>
{
	//The renderer manages the Camera
	typename RendererReprTrait::Camera m_Camera;

public:

	RendererRepr()
	{
		m_Camera.SetEyepointPosition(0,256,256);
	}

	void CameraClick()
	{
		//One Camera translation/rotation per click
		m_Camera.Click();

		//For culling tests, extracted once per Camera click
		ExtractPlanes();
	}

	inline void SaveViewFrustumState()
	{
		//Save the state of which Planes fail
		//the Sphere culling test
		FrustumPolicy<RendererReprTrait>::SaveState();
	}

	inline void RestoreViewFrustumState()
	{
		//Restore the state of the Planes
		FrustumPolicy<RendererReprTrait>::RestoreState();
	}


	inline void AddToSortBuffer(typename RendererReprTrait::DrawableObject* pModel,
								const typename RendererReprTrait::ScalingMatrix& Scaling, 
								const typename RendererReprTrait::RotationMatrix& Rotation, 
								const typename RendererReprTrait::TranslationMatrix& Translation) const
	{
		RenderPolicy<T,RendererReprTrait>::AddToSortBuffer(pModel,Scaling,Rotation,Translation);
	}

	bool IsBoundingRegionCulled(const typename RendererReprTrait::BoundingRegion& boundingregion) const;
};

template< typename T,
		  typename RendererReprTrait,
		  template <typename, typename> class FrustumPolicy,
		  template <typename, typename> class RenderPolicy,
		  template <typename, typename> class CullingPolicy>
bool RendererRepr<T,
				  RendererReprTrait,
				  FrustumPolicy,
				  RenderPolicy,
				  CullingPolicy>::IsBoundingRegionCulled(const typename RendererReprTrait::BoundingRegion& boundingregion) const
	{
		//Prefix these local variables with b, since there is a rogue "#define near" 
		//somewhere in the external source code
		bool bright; 
		bool bleft; 
		bool btop; 
		bool bbottom; 
		bool bnear; 
		bool bfar;

		RendererReprTrait::Plane plane;

		plane=GetRightPlane();
#ifdef _DEBUG
		glPointSize(10.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(1.0f,0,0);
		glVertex3f(plane.m_P.x+plane.m_N.x*20,plane.m_P.y+plane.m_N.y*20,plane.m_P.z+plane.m_N.z*20);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
#endif
		bright=IsCulled(boundingregion,plane);

		plane=GetLeft();
#ifdef _DEBUG
		glPointSize(10.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(0,0,1.0f);
		glVertex3f(plane.m_P.x+plane.m_N.x*20,plane.m_P.y+plane.m_N.y*20,plane.m_P.z+plane.m_N.z*20);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
#endif
		bleft=IsCulled(boundingregion,plane);

		plane=GetTop();
#ifdef _DEBUG
		glPointSize(10.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(1.0f,0,0);
		glVertex3f(plane.m_P.x+plane.m_N.x*40,plane.m_P.y+plane.m_N.y*40,plane.m_P.z+plane.m_N.z*40);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
#endif
		btop=IsCulled(boundingregion,plane);

		plane=GetBottom();
#ifdef _DEBUG
		glPointSize(10.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(1.0f,0,0);
		glVertex3f(plane.m_P.x+plane.m_N.x*40,plane.m_P.y+plane.m_N.y*40,plane.m_P.z+plane.m_N.z*40);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
#endif
		bbottom=IsCulled(boundingregion,plane);

		plane=GetNear();
#ifdef _DEBUG
		glPointSize(10.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(1.0f,0,0);
		glVertex3f(plane.m_P.x+plane.m_N.x*40,plane.m_P.y+plane.m_N.y*40,plane.m_P.z+plane.m_N.z*40);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
#endif
		bnear=IsCulled(boundingregion,plane);


		plane=GetFar();
#ifdef _DEBUG
		glPointSize(10.0f);
		glBegin(GL_POINTS);
		glColor3f(1.0f,1.0f,1.0f);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(1.0f,0,0);
		glVertex3f(plane.m_P.x+plane.m_N.x*40,plane.m_P.y+plane.m_N.y*40,plane.m_P.z+plane.m_N.z*40);
		glVertex3f(plane.m_P.x,plane.m_P.y,plane.m_P.z);
		glEnd();
#endif
		bfar=IsCulled(boundingregion,plane);


		bool isculled=(bright||bleft||btop||bbottom||bnear||bfar);	

		
		return isculled;
	}