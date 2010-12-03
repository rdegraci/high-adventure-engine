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

#include <cmath>


#include "TransformationTrait.h"
#include "MathTrait.h"
#include "GeometryTrait.h"
#include "Matrix.h"
#include "Vector.h"
#include "CoordinateSystem.h"
#include "Quaternion.h"


struct XAxis;
struct YAxis;
struct ZAxis;
struct ArbitraryAxis;

template<typename T,
		 typename TransformationTrait,
		 template <typename> class MathTrait>
void RotationMatrixToAngleAxis(const Matrix<T,3,3,Storage2Dim>& matrix,Vector3x1<float>& U, T& angleofrotation);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Rotation Template Class Declaration

template <typename T,
	      typename GeometryTrait,
		  typename TransformationTrait>
class Rotation : public Matrix<T,3,3,Storage2Dim>	
{
	//Purposely Empty
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Rotation Template Class Partial Instantiations


template <typename T,
          typename TransformationTrait>
class Rotation<T,XAxis,TransformationTrait>: public Matrix<T,3,3,Storage2Dim>
{
public:
	Rotation(T angleRadian)
	{
		m_Storage.M[0][0] = 1; m_Storage.M[0][1] = 0; m_Storage.M[0][2] = 0;

		m_Storage.M[1][0] = 0; m_Storage.M[1][1] = (float)cos(angleRadian); m_Storage.M[1][2] = (float)-sin(angleRadian);

		m_Storage.M[2][0] = 0; m_Storage.M[2][1] = (float)sin(angleRadian);	m_Storage.M[2][2] = (float)cos(angleRadian);

#ifdef _DEBUG
		//The Tranpose and the Inverse of the Rotation are the same Matrix
		Matrix<T,3,3,Storage2Dim> R1,R2;
		R1=R2=*this;
		R1.Invert();
		R2.Transpose();
		assert(R1==R2);
#endif _DEBUG
	}
};

template<typename T,
         typename TransformationTrait>
class Rotation<T,YAxis,TransformationTrait>: public Matrix<T,3,3,Storage2Dim>
{
public:
	Rotation(T angleRadian)
	{
		m_Storage.M[0][0] = static_cast<float>(cos(angleRadian));
		m_Storage.M[0][1] = T();
		m_Storage.M[0][2] = static_cast<float>(sin(angleRadian));

		m_Storage.M[1][0] = T();
		m_Storage.M[1][1] = 1;
		m_Storage.M[1][2] = T();

		m_Storage.M[2][0] = static_cast<float>(-sin(angleRadian));
		m_Storage.M[2][1] = T();
		m_Storage.M[2][2] = static_cast<float>(cos(angleRadian));

#ifdef _DEBUG
		//The Tranpose and the Inverse of the Rotation are the same Matrix
		Matrix<T,3,3,Storage2Dim> R1,R2;
		R1=R2=*this;
		R1.Invert();
		R2.Transpose();
		assert(R1==R2);
#endif _DEBUG
	}
};

template<typename T,
         typename TransformationTrait>
class Rotation<T,ZAxis,TransformationTrait>: public Matrix<T,3,3,Storage2Dim>
{
public:
	Rotation(T angleRadian)
	{
		m_Storage.M[0][0] = (float)cos(angleRadian);
		m_Storage.M[0][1] = (float)-sin(angleRadian);
		m_Storage.M[0][2] = 0;

		m_Storage.M[1][0] = (float)sin(angleRadian);
		m_Storage.M[1][1] = (float)cos(angleRadian);
		m_Storage.M[1][2] = 0;

		m_Storage.M[2][0] = 0;
		m_Storage.M[2][1] = 0;
		m_Storage.M[2][2] = 1;

#ifdef _DEBUG
		//The Tranpose and the Inverse of the Rotation are the same Matrix
		Matrix<T,3,3,Storage2Dim> R1,R2;
		R1=R2=*this;
		R1.Invert();
		R2.Transpose();
		assert(R1==R2);
#endif _DEBUG
	}
};

template<typename T,
         typename TransformationTrait>
class Rotation<T,ArbitraryAxis,TransformationTrait>: public Matrix<T,3,3,Storage2Dim>
{
public:

	//Angle-Axis to Rotation
	Rotation(const typename TransformationTrait::ColumnVector& U, const T& theta)
	{

#ifdef _DEBUG
		//U must be normalized
		typename TransformationTrait::ColumnVector U1(U);
		U1.Normalize();
		assert(U==U1);
#endif _DEBUG

		typename TransformationTrait::RotationMatrix S,I,R;
		
		S.m_Storage.M[0][0]=T(), S.m_Storage.M[0][1]=-U.z, S.m_Storage.M[0][2]=U.y;
		S.m_Storage.M[1][0]=U.z, S.m_Storage.M[1][1]=T(), S.m_Storage.M[1][2]=-U.x;
		S.m_Storage.M[2][0]=-U.y, S.m_Storage.M[2][1]=U.x, S.m_Storage.M[2][2]=T();

		I.SetIdentity();

		R=I+
			(sin(theta)*S)+
			((1-cos(theta))*
			(S*S));

		*this=R;

#ifdef _DEBUG
		//The Tranpose and the Inverse of the Rotation are the same Matrix
		Matrix<T,3,3,Storage2Dim> R1,R2;
		R1=R2=*this;
		assert(R1==R2);

		R1.Invert();
		R2.Transpose();
		assert(R1==R2);
		
		//S*V==UxV
		typename TransformationTrait::ColumnVector V(1.0,3.0,5.0);
		typename TransformationTrait::ColumnVector A(S*V);
		typename TransformationTrait::ColumnVector B(Cross<float,3,1>(U,V));
		assert(A==B);

		//R1*V==V+(1-cos(theta)*U) X V+(1-cos(theta)*U) X V+(U X V)
		R1=*this;
		typename TransformationTrait::ColumnVector V1=(R1*V);
		typename TransformationTrait::ColumnVector V2=((1-cos(theta))*U);
		typename TransformationTrait::ColumnVector V3=(V+
										Cross<float,3,1>((sin(theta)*U),V)+
										Cross<float,3,1>(V2,
										(Cross<float,3,1>(U,V))
										));
		assert(V1==V3);

#endif _DEBUG

	}

	Rotation(const Quaternion<T>& q)
	{
		m_Storage.M[0][0]=(1-(2*q.y*q.y)-(2*q.z*q.z)),m_Storage.M[0][1]=((2*q.x*q.y)+(2*q.w*q.z)),m_Storage.M[0][2]=((2*q.x*q.z)-(2*q.w*q.y));
		m_Storage.M[1][0]=((2*q.x*q.y)-(2*q.w*q.z)),m_Storage.M[1][1]=(1-(2*q.x*q.x)-(2*q.z*q.z)),m_Storage.M[1][2]=((2*q.y*q.z)+(2*q.w*q.x));
		m_Storage.M[2][0]=((2*q.x*q.z)+(2*q.w*q.y)),m_Storage.M[2][1]=((2*q.y*q.z)-(2*q.w*q.x)),m_Storage.M[2][2]=(1-(2*q.x*q.x)-(2*q.y*q.y));
	}

	Rotation<T,ArbitraryAxis,TransformationTrait>& operator=(const Rotation<T,ArbitraryAxis,TransformationTrait>& rotation)
{
	if (&rotation==this) return *this;

	Matrix<T,3,3,Storage2Dim>::operator=(rotation);
	
	return *this;
}

	Rotation<T,ArbitraryAxis,TransformationTrait>& operator=(const typename TransformationTrait::RotationMatrix& rotation)
{
	m_Storage.M[0][0]=rotation.m11,m_Storage.M[0][1]=rotation.m12,m_Storage.M[0][2]=rotation.m13;
	m_Storage.M[1][0]=rotation.m21,m_Storage.M[1][1]=rotation.m22,m_Storage.M[1][2]=rotation.m23;
	m_Storage.M[2][0]=rotation.m31,m_Storage.M[2][1]=rotation.m32,m_Storage.M[2][2]=rotation.m33;

	return *this;
}

};

/*
template<typename T>
Rotation<T,ArbitraryAxis,R3>& Rotation<T,ArbitraryAxis,R3>::operator=(const Rotation<T,ArbitraryAxis,R3>& rotation)
{
	if (&rotation==this) return *this;

	Matrix<T,3,3,Storage2Dim>::operator=(rotation);
	
	return *this;
}

template<typename T>
Rotation<T,ArbitraryAxis,R3>& Rotation<T,ArbitraryAxis,R3>::operator=(const R3<T>::RotationMatrix& rotation)
{
	m_Storage.M[0][0]=rotation.m11,m_Storage.M[0][1]=rotation.m12,m_Storage.M[0][2]=rotation.m13;
	m_Storage.M[1][0]=rotation.m21,m_Storage.M[1][1]=rotation.m22,m_Storage.M[1][2]=rotation.m23;
	m_Storage.M[2][0]=rotation.m31,m_Storage.M[2][1]=rotation.m32,m_Storage.M[2][2]=rotation.m33;

	return *this;
}


*/

template<typename T,
		 typename TransformationTrait,
		 template <typename> class MathTrait>
void RotationMatrixToAngleAxis(const Matrix<T,3,3,Storage2Dim>& matrix,Vector3x1<float>& U, T& theta)
{
	T trace(matrix.m_Storage.M[0][0]+matrix.m_Storage.M[1][1]+matrix.m_Storage.M[2][2]);
	T h=(trace-1)*0.5f;
	typename MathTrait<T>::Radian radian(static_cast<float>(acos(static_cast<double>(h))));
	if (radian>0)
	{
		if (radian<3.14159 /*MathTrait<T>::PI*/)
		{
			typename TransformationTrait::ColumnVector V;
			U.x=(matrix.m_Storage.M[2][1]-matrix.m_Storage.M[1][2]);
			U.y=(matrix.m_Storage.M[0][2]-matrix.m_Storage.M[2][0]);
			U.z=(matrix.m_Storage.M[1][0]-matrix.m_Storage.M[0][1]);

			U.Normalize();
		}
		else	//theta==PI
		{
			if (matrix.m_Storage.M[0][0]>=matrix.m_Storage.M[1][1])
			{
				if(matrix.m_Storage.M[0][0]>=matrix.m_Storage.M[2][2])
				{
					//	matrix.m_Storage.M[0][0] is the Maximum
					U.x=(0.5f*sqrt(matrix.m_Storage.M[0][0]-matrix.m_Storage.M[1][1]-matrix.m_Storage.M[2][2]+1));
					U.y=(matrix.m_Storage.M[0][1]/(2*U.x));
					U.z=(matrix.m_Storage.M[0][2]/(2*U.x));
				}
				else	
				{
					// matrix.m_Storage.M[2][2] is Maximum 
					U.z=(0.5f*sqrt(matrix.m_Storage.M[2][2]-matrix.m_Storage.M[0][0]-matrix.m_Storage.M[1][1]+1));
					U.x=(matrix.m_Storage.M[0][2]/(2*U.z));
					U.y=(matrix.m_Storage.M[1][2]/(2*U.z));
				}
			}
			else if (matrix.m_Storage.M[1][1]>=matrix.m_Storage.M[2][2])
			{
				// matrix.m_Storage.M[1][1] is the Maximum
				U.y=(0.5f*sqrt(matrix.m_Storage.M[1][1]-matrix.m_Storage.M[0][0]-matrix.m_Storage.M[2][2]+1));
				U.x=(matrix.m_Storage.M[0][1]/(2*U.y));
				U.z=(matrix.m_Storage.M[1][2]/(2*U.y));
			}
			else	
			{
				// matrix.m_Storage.M[2][2] is Maximum 
				U.z=(0.5f*sqrt(matrix.m_Storage.M[2][2]-matrix.m_Storage.M[0][0]-matrix.m_Storage.M[1][1]+1));
				U.x=(matrix.m_Storage.M[0][2]/(2*U.z));
				U.y=(matrix.m_Storage.M[1][2]/(2*U.z));
			}
		}
		theta=radian;
	}
	else	//theta==0.0
	{
		U.x=T();
		U.y=T();
		U.z=T()+1;

		theta=0.0;
	}
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//HomogenousTransform Template Class Declaration

template <typename T,
		  typename TransformationTrait>
class HomogeneousTransform
{
public:
	typename TransformationTrait::RotationMatrix m_M;
	typename TransformationTrait::ColumnVector m_T;
	typename TransformationTrait::RowVector m_S;
	typename TransformationTrait::One m_m44;

	HomogeneousTransform() : m_m44(1)
	{
	}

	HomogeneousTransform(const HomogeneousTransform<T,TransformationTrait>& ht);
	~HomogeneousTransform();

	HomogeneousTransform<T,TransformationTrait>& operator=(const HomogeneousTransform<T,TransformationTrait>& ht);

	void Invert()
	{
		typename TransformationTrait::RotationMatrix M(m_M);
		M.Invert();

		m_M=M;
		m_T=-M*m_T;
	}

};



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//HomogenousTransform Member Template Definitions

template <typename T,
		  typename TransformationTrait>
HomogeneousTransform<T,TransformationTrait>::HomogeneousTransform(const HomogeneousTransform<T,TransformationTrait>& ht)
{
	m_M=ht.m_M;
	m_T=ht.m_T;
	m_S=ht.m_S;
	m_m44=ht.m_m44;
}

template <typename T,
		  typename TransformationTrait>
HomogeneousTransform<T,TransformationTrait>::~HomogeneousTransform()
{
}

template <typename T,
		  typename TransformationTrait>
HomogeneousTransform<T,TransformationTrait>& HomogeneousTransform<T,TransformationTrait>::operator=(const HomogeneousTransform<T,TransformationTrait>& ht)
{
	if(this==&ht) return *this;

	m_M=ht.m_M;
	m_T=ht.m_T;
	m_S=ht.m_S;
	m_m44=ht.m_m44;	

	return *this;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Helper Function Definitions

//HT*HT
template <typename T,
		  typename TransformationTrait>
HomogeneousTransform<T,TransformationTrait> operator*(const HomogeneousTransform<T,TransformationTrait>& ht1, const HomogeneousTransform<T,TransformationTrait>& ht2);


template <typename T,
		  typename TransformationTrait>
HomogeneousTransform<T,TransformationTrait> operator*(const HomogeneousTransform<T,TransformationTrait>& ht1, const HomogeneousTransform<T,TransformationTrait>& ht2)
{
	HomogeneousTransform<T,TransformationTrait> ret;
    ret.m_M.m_Storage.M[0][0] = ht1.m_M.m_Storage.M[0][0] * ht2.m_M.m_Storage.M[0][0] + ht1.m_M.m_Storage.M[0][1] * ht2.m_M.m_Storage.M[1][0] + ht1.m_M.m_Storage.M[0][2] * ht2.m_M.m_Storage.M[2][0] + ht1.m_T.m_Storage.M[0][0] * ht2.m_S.m_Storage.M[0][0];
    ret.m_M.m_Storage.M[0][1] = ht1.m_M.m_Storage.M[0][0] * ht2.m_M.m_Storage.M[0][1] + ht1.m_M.m_Storage.M[0][1] * ht2.m_M.m_Storage.M[1][1] + ht1.m_M.m_Storage.M[0][2] * ht2.m_M.m_Storage.M[2][1] + ht1.m_T.m_Storage.M[0][0] * ht2.m_S.m_Storage.M[0][1];
    ret.m_M.m_Storage.M[0][2] = ht1.m_M.m_Storage.M[0][0] * ht2.m_M.m_Storage.M[0][2] + ht1.m_M.m_Storage.M[0][1] * ht2.m_M.m_Storage.M[1][2] + ht1.m_M.m_Storage.M[0][2] * ht2.m_M.m_Storage.M[2][2] + ht1.m_T.m_Storage.M[0][0] * ht2.m_S.m_Storage.M[0][2];
    ret.m_T.m_Storage.M[0][0] = ht1.m_M.m_Storage.M[0][0] * ht2.m_T.m_Storage.M[0][0] + ht1.m_M.m_Storage.M[0][1] * ht2.m_T.m_Storage.M[1][0] + ht1.m_M.m_Storage.M[0][2] * ht2.m_T.m_Storage.M[2][0] + ht1.m_T.m_Storage.M[0][0] * ht2.m_m44;

    ret.m_M.m_Storage.M[1][0] = ht1.m_M.m_Storage.M[1][0] * ht2.m_M.m_Storage.M[0][0] + ht1.m_M.m_Storage.M[1][1] * ht2.m_M.m_Storage.M[1][0] + ht1.m_M.m_Storage.M[1][2] * ht2.m_M.m_Storage.M[2][0] + ht1.m_T.m_Storage.M[1][0] * ht2.m_S.m_Storage.M[0][0];
    ret.m_M.m_Storage.M[1][1] = ht1.m_M.m_Storage.M[1][0] * ht2.m_M.m_Storage.M[0][1] + ht1.m_M.m_Storage.M[1][1] * ht2.m_M.m_Storage.M[1][1] + ht1.m_M.m_Storage.M[1][2] * ht2.m_M.m_Storage.M[2][1] + ht1.m_T.m_Storage.M[1][0] * ht2.m_S.m_Storage.M[0][1];
    ret.m_M.m_Storage.M[1][2] = ht1.m_M.m_Storage.M[1][0] * ht2.m_M.m_Storage.M[0][2] + ht1.m_M.m_Storage.M[1][1] * ht2.m_M.m_Storage.M[1][2] + ht1.m_M.m_Storage.M[1][2] * ht2.m_M.m_Storage.M[2][2] + ht1.m_T.m_Storage.M[1][0] * ht2.m_S.m_Storage.M[0][2];
    ret.m_T.m_Storage.M[1][0] = ht1.m_M.m_Storage.M[1][0] * ht2.m_T.m_Storage.M[0][0] + ht1.m_M.m_Storage.M[1][1] * ht2.m_T.m_Storage.M[1][0] + ht1.m_M.m_Storage.M[1][2] * ht2.m_T.m_Storage.M[2][0] + ht1.m_T.m_Storage.M[1][0] * ht2.m_m44;

    ret.m_M.m_Storage.M[2][0] = ht1.m_M.m_Storage.M[2][0] * ht2.m_M.m_Storage.M[0][0] + ht1.m_M.m_Storage.M[2][1] * ht2.m_M.m_Storage.M[1][0] + ht1.m_M.m_Storage.M[2][2] * ht2.m_M.m_Storage.M[2][0] + ht1.m_T.m_Storage.M[2][0] * ht2.m_S.m_Storage.M[0][0];
    ret.m_M.m_Storage.M[2][1] = ht1.m_M.m_Storage.M[2][0] * ht2.m_M.m_Storage.M[0][1] + ht1.m_M.m_Storage.M[2][1] * ht2.m_M.m_Storage.M[1][1] + ht1.m_M.m_Storage.M[2][2] * ht2.m_M.m_Storage.M[2][1] + ht1.m_T.m_Storage.M[2][0] * ht2.m_S.m_Storage.M[0][1];
    ret.m_M.m_Storage.M[2][2] = ht1.m_M.m_Storage.M[2][0] * ht2.m_M.m_Storage.M[0][2] + ht1.m_M.m_Storage.M[2][1] * ht2.m_M.m_Storage.M[1][2] + ht1.m_M.m_Storage.M[2][2] * ht2.m_M.m_Storage.M[2][2] + ht1.m_T.m_Storage.M[2][0] * ht2.m_S.m_Storage.M[0][2];
    ret.m_T.m_Storage.M[2][0] = ht1.m_M.m_Storage.M[2][0] * ht2.m_T.m_Storage.M[0][0] + ht1.m_M.m_Storage.M[2][1] * ht2.m_T.m_Storage.M[1][0] + ht1.m_M.m_Storage.M[2][2] * ht2.m_T.m_Storage.M[2][0] + ht1.m_T.m_Storage.M[2][0] * ht2.m_m44;

    ret.m_S.m_Storage.M[0][0] = ht1.m_S.m_Storage.M[0][0] * ht2.m_M.m_Storage.M[0][0] + ht1.m_S.m_Storage.M[0][1] * ht2.m_M.m_Storage.M[1][0] + ht1.m_S.m_Storage.M[0][2] * ht2.m_M.m_Storage.M[2][0] + ht1.m_m44 * ht2.m_S.m_Storage.M[0][0];
    ret.m_S.m_Storage.M[0][1] = ht1.m_S.m_Storage.M[0][0] * ht2.m_M.m_Storage.M[0][1] + ht1.m_S.m_Storage.M[0][1] * ht2.m_M.m_Storage.M[1][1] + ht1.m_S.m_Storage.M[0][2] * ht2.m_M.m_Storage.M[2][1] + ht1.m_m44 * ht2.m_S.m_Storage.M[0][1];
    ret.m_S.m_Storage.M[0][2] = ht1.m_S.m_Storage.M[0][0] * ht2.m_M.m_Storage.M[0][2] + ht1.m_S.m_Storage.M[0][1] * ht2.m_M.m_Storage.M[1][2] + ht1.m_S.m_Storage.M[0][2] * ht2.m_M.m_Storage.M[2][2] + ht1.m_m44 * ht2.m_S.m_Storage.M[0][2];
    ret.m_m44 = ht1.m_S.m_Storage.M[0][0] * ht2.m_T.m_Storage.M[0][0] + ht1.m_S.m_Storage.M[0][1] * ht2.m_T.m_Storage.M[1][0] + ht1.m_S.m_Storage.M[0][2] * ht2.m_T.m_Storage.M[2][0] + ht1.m_m44 * ht2.m_m44;

    return ret;
}


		 

