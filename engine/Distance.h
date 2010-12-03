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

#include "Vector.h"
#include "Geometry.h"
#include "GeometryTrait.h"
#include "ComponentTrait.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Forward Declarations

template<typename T>
class Vector1x3;

template<typename T>
class Vector3x1;

template <typename T,
		  typename ComponentTrait,
		  typename GeometryTrait=StandardEuclidean>
class LinearComponent;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Distance Calculation Function Template Definitions


template <typename T,
		  typename TransformationTrait>
T CalcDistance(const HomogeneousCoordinate<T,TransformationTrait>& point1, const HomogeneousCoordinate<T,TransformationTrait>& point2)
{
	HomogeneousCoordinate<T,TransformationTrait> P(point1-point2);
	return P.Magnitude();
}

template <typename T>
T CalcDistance(const Vector3x1<T>& point1, const Vector3x1<T>& point2)
{
	Vector3x1<T> P(point1-point2);
	return P.Magnitude();
}

/*
template<typename T>
T CalcDistance(const Point<T,GeometryTrait>& point, const LinearComponent<T,Ray>& linearcomponent)
{
	T t0=Dot<float,3,3>(linearcomponent.m_M,(point.m_P-linearcomponent.m_B))/
		 Dot<float>(linearcomponent.m_M,linearcomponent.m_M);

	if(t0<=0.0f)	//Behind the Ray startpoint
	{
		typename GeometryTrait::Vector v(point.m_P-linearcomponent.m_B);
		return (v.Magnitude());
	}
	else	//Anywhere in front of the Ray startpoint
	{
		typename GeometryTrait::Vector v(point.m_P-(linearcomponent.m_B-(t0*linearcomponent.m_M)));
		return (v.Magnitude());
	}
}

template<typename T,
		 typename GeometryTrait>
T CalcDistance(const Point<T>& point, const LinearComponent<T,Segment>& linearcomponent)
{
	T t0=Dot<float>(linearcomponent.m_M,(point.m_P-linearcomponent.m_B))/
		 Dot<float>(linearcomponent.m_M,linearcomponent.m_M);

	if(t0<=0.0f)	//Behind the starting point of the segment
	{
		typename GeometryTrait::Vector v(point.m_P-linearcomponent.m_B);
		return (v.Magnitude());
	}
	else if (t0>0 && t0<1)	//Within the start and endpoints of the segment
	{
		typename GeometryTrait::Vector v(point.m_P-(linearcomponent.m_B-(t0*linearcomponent.m_M)));
		return (v.Magnitude());
	}
	else	//Beyond the endpoint of the segment
	{
		typename GeometryTrait::Vector v(point.m_P-(linearcomponent.m_B-linearcomponent.m_M)));
		return (v.Magnitude());
	}
}

template<typename T,
		 typename class GeometryTrait>
T CalcDistance(const Point<T>& point, const LinearComponent<T,Line>& linearcomponent)
{
	T t0=Vector<float,3,1>::Dot(linearcomponent.m_M,(point.m_P-linearcomponent.m_B))/
		 Vector<float,3,1>::Dot(linearcomponent.m_M,linearcomponent.m_M);

	//Since we have a line, we don't care if t0 is positive, negative, or zero
	typename GeometryTrait::Vector v(point.m_P-(linearcomponent.m_B-(t0*linearcomponent.m_M)));
	return (v.Magnitude());
}


template<typename T,
		 typename GeometryTrait>
T CalcDistance(const LinearComponent<T,Line>& lc0, const LinearComponent<T,Line>& lc1)
{
	T a=Dot<T>(lc0.m_M,lc0.m_M);
	T b=-(Dot<T>(lc0.m_M,lc1.m_M));
	T c=Dot<T>(lc1.m_M,lc1.m_M);
	T d=Dot<T>(lc0.m_M,(lc0.m_B-lc1.m_B));
	T e=-(Dot<T>(lc1.m_M,(lc0.m_B-lc1.m_B)));
	T f=Dot<T>((lc0.m_B-lc1.m_B),(lc0.m_B-lc1.m_B));

	T det=abs(a*c-b*b);	
	
	//Always greaterthan or equal to zero
	assert((det>0)||(det==0));


	//TODO:
	//Check accuracy of det


	if (det>0)	//Lines are not parallel
	{
		float invdet=1/det;

		s=(b*e-c*d)*invdet;
		t=(b*d-a*e)*invdet;

		return s*(a*s+b*t+2*d)+t*(b*s+c*t+2*e)+f;
	}
	else //Lines are parallel
	{
		s=-d/a;
		t=0;

		return d*s+f;
	}
}
*/


