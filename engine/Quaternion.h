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
#include "Transform.h"
#include "GeometryTrait.h"
#include "TransformationTrait.h"
#include "StorageTrait.h"

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
class Vector;

template<typename T>
class Vector1x3;

template<typename T>
class Vector3x1;

template <typename T,
	      typename GeometryTrait,
		  typename TransformationTrait>
class Rotation;

struct ArbitraryAxis;
struct Real3;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Quaternion Template Class Declaration


template <typename T>
class Quaternion
{
public:
	T x,y,z,w;

	Quaternion() : w(T()),x(T()),y(T()),z(T())
	{
	}

	Quaternion(T ww, T xx, T yy, T zz) : w(ww),x(xx),y(yy),z(zz)
	{
	}

	Quaternion(const Vector<T,3,1,Storage2Dim>& v);
	Quaternion(const Matrix<T,3,3,Storage2Dim>& m);
	Quaternion(const Vector<T,3,1,Storage2Dim>& U, const T& theta);
	Quaternion(const Rotation<T,ArbitraryAxis,Real3>& R);
	Quaternion(const Quaternion<T>& quat);
	virtual ~Quaternion();

	Quaternion<T>& operator=(const Quaternion<T>& quat);


	//We do not support the Conjugate
	//The Conjugate of a Quaterion is imaginative

	T Norm() {return (w*w+x*x+y*y+z*z);}

	//We do not support MultiplicativeInverse 
	//because it is the Conjugate divided by the Norm, which is imaginative

	T Selection() { return w; }

	static T Dot(const Quaternion<T>& q0, const Quaternion<T>& q1);

	void ToAngleAxis(Vector<T,3,1,Storage2Dim>& U, T& theta);
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Helper Template Function  Declaration

template <typename T>
Quaternion<T> Slerp(const Quaternion<T> &quat1, const Quaternion<T> &quat2, float slerp);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Quaternion Member Template Definitions

template <typename T>
Quaternion<T>::Quaternion(const Vector<T,3,1,Storage2Dim>& v) :
	w(0),x(v.m_Storage.M[0]),y(v.m_Storage.M[1]),z(v.m_Storage.M[2])
{
}


template <typename T>
Quaternion<T>::Quaternion(const Rotation<T,ArbitraryAxis,Real3>& R)
{
	T trace=R.m_Storage.M[0][0]+R.m_Storage.M[1][1]+R.m_Storage.M[2][2];

	if (trace>0)
	{
		w=0.5*(sqrt(trace+1));
		x=((R.m_Storage.M[1][2]-R.m_Storage.M[2][1])/(4*w));
		y=((R.m_Storage.M[2][0]-R.m_Storage.M[0][2])/(4*w));
		z=((R.m_Storage.M[0][1]-R.m_Storage.M[1][0])/(4*w));
		return;
	}
	if (trace<=0)
	{
		if (R.m_Storage.M[0][0]>=R.m_Storage.M[1][1])
		{
			if(R.m_Storage.M[0][0]>=R.m_Storage.M[2][2])
			{
				//	R.m_Storage.M[0][0] is the Maximum
				x=(0.5*sqrt(matrix.m_Storage.M[0][0]-matrix.m_Storage.M[1][1]-matrix.m_Storage.M[2][2]+1));
				w=((R.m_Storage.M[1][2]-R.m_Storage.M[2][1])/(4*x));
				y=((R.m_Storage.M[0][1]+R.m_Storage.M[1][0])/(4*x));
				z=((R.m_Storage.M[0][2]+R.m_Storage.M[2][0])/(4*x));
			}
			else	
			{
				// R.m_Storage.M[2][2] is Maximum 
				z=(0.5*sqrt(matrix.m_Storage.M[2][2]-matrix.m_Storage.M[0][0]-matrix.m_Storage.M[1][1]+1));
				w=((R.m_Storage.M[0][1]-R.m_Storage.M[1][0])/(4*z));
				x=((R.m_Storage.M[0][2]+R.m_Storage.M[2][0])/(4*z));
				y=((R.m_Storage.M[1][2]+R.m_Storage.M[2][1])/(4*z));
			}
		}
		else if (R.m_Storage.M[1][1]>=R.m_Storage.M[2][2])
		{
			// R.m_Storage.M[1][1] is the Maximum
			y=(0.5*sqrt(matrix.m_Storage.M[1][1]-matrix.m_Storage.M[0][0]-matrix.m_Storage.M[2][2]+1));
			w=((R.m_Storage.M[2][0]-R.m_Storage.M[0][2])/(4*y));
			x=((R.m_Storage.M[0][1]+R.m_Storage.M[1][0])/(4*y));
			z=((R.m_Storage.M[1][2]+R.m_Storage.M[2][1])/(4*y));

		}
		else	
		{
			// R.m_Storage.M[2][2] is Maximum 
			z=(0.5*sqrt(matrix.m_Storage.M[2][2]-matrix.m_Storage.M[0][0]-matrix.m_Storage.M[1][1]+1));
			w=((R.m_Storage.M[0][1]-R.m_Storage.M[1][0])/(4*z));
			x=((R.m_Storage.M[0][2]+R.m_Storage.M[2][0])/(4*z));
			y=((R.m_Storage.M[1][2]+R.m_Storage.M[2][1])/(4*z));
		}
	}
}

template <typename T>
Quaternion<T>::Quaternion(const Vector<T,3,1,Storage2Dim>& U, const T& theta)
{
	w=cos(theta/2);
	x=U.x*sin(theta/2);
	y=U.y*sin(theta/2);
	z=U.z*sin(theta/2);
}

template <typename T>
void Quaternion<T>::ToAngleAxis(Vector<T,3,1,Storage2Dim>& U, T& theta)
{
	T s(fabs(w));

	if (s==1)
	{
		theta=T();
		U.x=T();
		U.y=T();
		U.z=1.0;
		return;
	}

	if (s<1)
	{
		theta=2*acos(w);
		U=U/(sqrt(1-(w*w)));
	}
}

template <typename T>
Quaternion<T>::Quaternion(const Matrix<T,3,3,Storage2Dim>& m)
{
	//Rotation Matrix to 
    //Copyright (C) 1998,1999,2000 by Jorrit Tyberghein
    //Largely rewritten by Ivan Avramovic <ivan@avramovic.com>
	//Crystal Space

    float  tr, s;
    int    i;



    tr = m.m_Storage.M[0][0] + m.m_Storage.M[1][1] + m.m_Storage.M[2][2] + 1.0f;

    // check the diagonal
    if (tr > 0.0f) {
        s = 0.5f / sqrt (tr);
        w = 0.25f / s;
        x = (m.m_Storage.M[2][1] - m.m_Storage.M[1][2]) * s;
        y = (m.m_Storage.M[0][2] - m.m_Storage.M[2][0]) * s;
        z = (m.m_Storage.M[1][0] - m.m_Storage.M[0][1]) * s;
    } else {
        // diagonal is negative
        i = 1;
        if (m.m_Storage.M[1][1] > m.m_Storage.M[0][0]) i = 2;
        if ((i == 1 && m.m_Storage.M[2][2] > m.m_Storage.M[0][0])
            || (i == 2 && m.m_Storage.M[2][2] > m.m_Storage.M[1][1])) i = 3;

            /*
            m11 = 0
            m12 = 1
            m13 = 2
            m21 = 4
            m22 = 5
            m23 = 6
            m31 = 8
            m32 = 9
            m33 = 10
            */

        switch(i) {
        case 1:
            s = sqrt ((m.m_Storage.M[0][0] - (m.m_Storage.M[1][1] + m.m_Storage.M[2][2])) + 1.0f);

            x = s * 0.5f;

            if (s != 0.0f) s = 0.5f / s;

            y = (m.m_Storage.M[0][1] + m.m_Storage.M[1][0]) * s;
            z = (m.m_Storage.M[0][2] + m.m_Storage.M[2][0]) * s;
            w = (m.m_Storage.M[1][2] - m.m_Storage.M[2][1]) * s;
            break;
        case 2:
            s = sqrt ((m.m_Storage.M[1][1] - (m.m_Storage.M[2][2] + m.m_Storage.M[0][0])) + 1.0f);

            y = 0.5f * s;

            if (s != 0.0f) s = 0.5f / s;

            x = (m.m_Storage.M[0][1] + m.m_Storage.M[1][0]) * s;
            z = (m.m_Storage.M[1][2] + m.m_Storage.M[2][1]) * s;
            w = (m.m_Storage.M[0][2] - m.m_Storage.M[2][0]) * s;

            break;
        case 3:
            s = sqrt ((m.m_Storage.M[2][2] - (m.m_Storage.M[0][0] + m.m_Storage.M[1][1])) + 1.0f);

            z = 0.5f * s;

            if (s != 0.0f) s = 0.5f / s;

            x = (m.m_Storage.M[0][2] + m.m_Storage.M[2][0]) * s;
            y = (m.m_Storage.M[1][2] + m.m_Storage.M[2][1]) * s;
            w = (m.m_Storage.M[0][1] - m.m_Storage.M[1][0]) * s;
            break;
        }
    }
}

template <typename T>
Quaternion<T>::Quaternion(const Quaternion<T>& quat)
{
	x=quat.x;
	y=quat.y;
	z=quat.z;
	w=quat.w;
}

template <typename T>
Quaternion<T>::~Quaternion()
{
}

template <typename T>
Quaternion<T>& Quaternion<T>::operator=(const Quaternion<T>& quat)
{
	if(this==&quat) return *this;

	x=quat.x;
	y=quat.y;
	z=quat.z;
	w=quat.w;

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Helper Template Function Definition

//Copyright (C) 1998,1999,2000 by Jorrit Tyberghein
//Largely rewritten by Ivan Avramovic <ivan@avramovic.com>

#define PI 3.12
#define SLERPDELTA  0.0001f 
// Difference at which to lerp instead of slerp

template <typename T>
Quaternion<T> Slerp(const Quaternion<T> &quat1, const Quaternion<T> &quat2, float slerp)
{
  double omega, cosom, invsinom, scale0, scale1;

  // Calculate dot between quats
  cosom = quat1.x * quat2.x + quat1.y * quat2.y + quat1.z * quat2.z + quat1.w * quat2.w;

  // Make sure the two quaternions are not exactly opposite? (within a little
  // slop).
  if ((1.0f + cosom) > SLERPDELTA)
  {
    // Are they more than a little bit different?  Avoid a divided by zero
    // and lerp if not.
    if ((1.0f - cosom) > SLERPDELTA)
    {
      // Yes, do a slerp
      omega = acos (cosom);
      invsinom = 1.0f / sin (omega);
      scale0 = sin ((1.0f - slerp) * omega) * invsinom;
      scale1 = sin (slerp * omega) * invsinom;
    }
    else
    {
      // Not a very big difference, do a lerp
      scale0 = 1.0f - slerp;
      scale1 = slerp;
    }

    return Quaternion<T>(
        scale0 * quat1.w + scale1 * quat2.w,
        scale0 * quat1.x + scale1 * quat2.x,
        scale0 * quat1.y + scale1 * quat2.y,
        scale0 * quat1.z + scale1 * quat2.z);
  }

  // The quaternions are nearly opposite so to avoid a divided by zero error
  // Calculate a perpendicular quaternion and slerp that direction
  scale0 = sin ((1.0f - slerp) * PI);
  scale1 = sin (slerp * PI);
  return Quaternion<T> (
      scale0 * quat1.w + scale1 * quat2.z,
      scale0 * quat1.x + scale1 * -quat2.y,
      scale0 * quat1.y + scale1 * quat2.x,
      scale0 * quat1.z + scale1 * -quat2.w);
}


////
	//Adding two Quaternions is imaginative
	//Subtracting two Quaternions is imaginative

	//Multiplying two Quaternions is imaginative, however
	//it can be simulated by substituting a vector for xi+yj+zk

template <typename T>
Quaternion<T> operator+(const Quaternion<T>& q0, const Quaternion<T>& q1);

template <typename T>
Quaternion<T> operator-(const Quaternion<T>& q0, const Quaternion<T>& q1);

////
template <typename T>
Quaternion<T> operator+(const Quaternion<T>& q0, const Quaternion<T>& q1)
{
	Quaterion<T> q;

	q.w=q0.w+q1.w;
	q.x=q0.x+q1.x;
	q.y=q0.y+q1.y;
	q.z=q0.z+q1.z;

	return q;
}

template <typename T>
Quaternion<T> operator-(const Quaternion<T>& q0, const Quaternion<T>& q1)
{
	Quaterion<T> q;

	q.w=q0.w-q1.w;
	q.x=q0.x-q1.x;
	q.y=q0.y-q1.y;
	q.z=q0.z-q1.z;

	return q;
}

template <typename T>
T Quaternion<T>::Dot(const Quaternion<T>& q0, const Quaternion<T>& q1)
{
	return (q0.w*q1.w+q0.x*q1.x+q0.y*q1.y+q0.z*q0.z);
}

