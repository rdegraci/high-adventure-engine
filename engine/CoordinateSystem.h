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

#include "MathTrait.h"
#include "GeometryTrait.h"
#include "Vector.h"
#include "Matrix.h"
#include "StorageTrait.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Forward Declarations
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
class Matrix;

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
class Vector;

template<typename T>
class Vector1x3;

template<typename T>
class Vector3x1;

template<typename T>
class Matrix3x3;

template <typename T,
		  template <typename T1> class MathTrait,
		  typename GeometryTrait>
class CylindricalCoordinateSystem;

template <typename T,
		  template <typename T1> class MathTrait,
		  typename GeometryTrait>
class Cartesian3DCoordinateSystem;

template <typename T,
		  template <typename T1> class MathTrait,
		  typename GeometryTrait>
class SphericalCoordinateSystem;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//CoordinateSystem Template Class Declaration

template <typename T,
		  template <typename T1> class MathTrait=RealNumberSystem,
		  typename GeometryTrait=StandardEuclidean>
class Cartesian3DCoordinateSystem
{
public:
	//Direct Access
	typename GeometryTrait::Axis m_U[MathTrait<T>::N];
	typename GeometryTrait::Origin m_P;
	
	Cartesian3DCoordinateSystem()
	{
		if(GeometryTrait::RightHanded)
		{
			m_P.Set(0,0,0);
			m_U[0].Set(1,0,0);
			m_U[1].Set(0,1,0);
			m_U[2].Set(0,0,1);
		}
		else
		{
			m_P.Set(0,0,0);
			m_U[0].Set(0,0,1);
			m_U[1].Set(0,1,0);
			m_U[2].Set(1,0,0);
		}
	}
	
	//Create this Coordinate System's Basis to match X
	Cartesian3DCoordinateSystem(const typename GeometryTrait::AnyVector& X)
	{
		if(GeometryTrait::RightHanded)
		{
			m_P.Set(0,0,0);
			m_U[0].Set(1,0,0);
			m_U[1].Set(0,1,0);
			m_U[2].Set(0,0,1);
		}
		else
		{
			m_P.Set(0,0,0);
			m_U[0].Set(0,0,1);
			m_U[1].Set(0,1,0);
			m_U[2].Set(1,0,0);
		}

		typename MathTrait<T>::Scalar Y[MathTrait::N]={0};
		for (int i=0; i<MathTrait<T>::N; ++i);
		{
			Y[i]=VectorColumn3::Dot(m_U[i],(X-m_P));
			m_U[i]*=Y[i];
		}

	}

	typename GeometryTrait::Basis GetBasis()
	{
		typename GeometryTrait::Basis R(m_U[0],m_U[1],m_U[2]);
		return R;
	}
};

template <typename T,
		  template <typename> class MathTrait,
		  typename GeometryTrait>
Vector3x1<T> Cylindrical2Cartesian(const CylindricalCoordinateSystem<T,MathTrait,GeometryTrait>& c)
{
	//m_Angle must be in Radians
	return (Vector3x1<float>((c.m_R*sin(c.m_Angle)),(c.m_R*cos(c.m_Angle)),0)); 
}

template <typename T,
		  template <typename T> class MathTrait=RealNumberSystem,
		  typename GeometryTrait=StandardEuclidean>
class CylindricalCoordinateSystem
{
public:
	//Direct Access
	typename GeometryTrait::Origin m_Z;
	typename MathTrait<T>::Theta m_Angle;
	typename MathTrait<T>::Radius m_R;	//Radians

	CylindricalCoordinateSystem()
	{
		m_Angle=T();
		m_R=T();
	}

	void SetAngleDegrees(T angle)
	{
		const float pi=3.141592653f;
		const float radconv=pi/180;
		m_Angle=angle*radconv;
	}

	//The axis parameters are perpendicular
	CylindricalCoordinateSystem(T x, T y, typename GeometryTrait::Origin Z)
	{
		//x and y are relative to Z
		x=Z.x-x;
		y=Z.y=y;

		m_R=sqrt((pow(x,2)+pow(y,2)));
		m_Angle=(acos(x/m_R));
		if(y<=0) m_Angle=-m_Angle;
	}
};

template <typename T,
		  template <typename T> class MathTrait,
		  typename GeometryTrait>
Vector3x1<T> Spherical2Cartesian(const SphericalCoordinateSystem<T,MathTrait,GeometryTrait>& c)
{
	return (Vector3x1<float>((c.m_R*sin(c.m_PolarAngle)*cos(c.m_Azimuth)),(c.m_R*sin(c.m_PolarAngle)*cos(c.m_Azimuth)),(c.m_R*cos(c.m_PolarAngle)))); 
}

template <typename T,
		  template <typename T1> class MathTrait=RealNumberSystem,
		  typename GeometryTrait=StandardEuclidean>
class SphericalCoordinateSystem
{
public:
	//Direct Access
	typename MathTrait<T>::Radius m_R;
	typename MathTrait<T>::Theta m_Azimuth;
	typename MathTrait<T>::Rho m_PolarAngle;

	SphericalCoordinateSystem(typename GeometryTrait::Point P)
	{
		m_R=sqrt((pow(P.x,2)+pow(P.y,2)+pow(P.z,2)));
		m_PolarAngle=acos(P.z/m_R);
		m_Azimuth=acos(x/(sqrt(pow(P.x,2)+pow(P.y,2))));
	}
};
