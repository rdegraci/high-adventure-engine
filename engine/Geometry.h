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

#include "GeometryTrait.h"
#include "MathReprPolicy.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Forward Declarations

template <typename T,
		  typename GeometryTrait>
class LinearComponent;

template <typename T,
		  typename GeometryTrait>
class Plane;

////
template <typename T,
		  template <typename> class MathReprPolicy=Cartesian>
class Point : public MathReprPolicy<T>
{
public:
	Point()
	{
	}

	Point(T x, T y, T z) : MathReprPolicy<T>(x,y,z)
	{

	}
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//LinearComponent Template Class Definition

template <typename T,
		  typename GeometryTrait=StandardEuclidean>
class LinearComponent
{
public:
	//Direct access
	typename GeometryTrait::Point m_B;
	typename GeometryTrait::ColumnVector m_M;

	LinearComponent(typename const GeometryTrait::Point& B, typename const GeometryTrait::ColumnVector& M) : 
	m_B(B), m_M(M)
	{
		m_M.Normalize();
	}
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Plane Template Class Definition

template <typename T,
		  typename GeometryTrait=StandardEuclidean>
class Plane
{

public:
	//Three dimensional representation
	typename GeometryTrait::Point m_P;
	typename GeometryTrait::Normal m_N;

	//Four dimensional representation
	typename GeometryTrait::Tuple4	m_ND;	// <N,D>

	Plane() : m_P(),m_N() 
	{
		
	}

	Plane(const T& px, const T& py, const T& pz,
		  const T& nx, const T& ny, const T& nz, const T& d) : m_P(px,py,pz),m_N(nx,ny,nz),m_ND(nx,ny,nz,d)
	{
		m_N.Normalize();

		assert( (Vector<float,3,1>::Dot(m_N,m_P) + (m_ND.m_Storage.M[3][0])==0));
	}

	Plane(const T& A, const T& B, const T& C, const T& D) : m_N(A,B,C), m_P(), m_ND(A,B,C,D)
	{
		assert(m_N.Magnitude()>0);
 
		(m_ND.m_Storage.M[3][0])/=m_N.Magnitude();
		m_N.Normalize();
 
		//Create a dummy Point
		m_P=m_N * -m_ND.m_Storage.M[3][0];
 
		//D==-(N dot P)
		T ndotp=(Vector<T,3,1>::Dot(m_N,m_P));
		T d=m_ND.m_Storage.M[3][0];
		assert(fabs(d + ndotp) < 0.001f); 
 
		// N dot P + D == 0
		T dd=(Vector<T,3,1>::Dot(m_N,m_P) + (m_ND.m_Storage.M[3][0]));
		assert(fabs(dd) < 0.001f);	//dd==zero
	}

	void SetNormal(const T& x, const T& y, const T& z)
	{
		m_N.Set(x,y,z);

		m_N.Normalize();

		m_ND.Set(m_N.x,m_N.y,m_N.z,0);	//Direction
	}

	void SetPoint(const T& x, const T& y, const T& z)
	{
		m_P.Set(x,y,z);
	}

	void SetD(const T& d)
	{
		m_ND.m_Storage.M[3][0]=d;
	}

	T GetD() const
	{
		return m_ND.m_Storage.M[3][0];
	}

	Plane(typename const GeometryTrait::Point& P, typename const GeometryTrait::Normal& N) : m_P(P), m_N(N)
	{
		//Make calculations easier by normalizing
		m_N.Normalize();

		m_ND.Set(N.x,N.y,N.z,-(Vector<float,3,1>::Dot(m_N,P)));

		assert( (Vector<float,3,1>::Dot(m_N,m_P) + (m_ND.m_Storage.M[3][0])==0));
	}
};

