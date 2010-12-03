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

#include <assert.h>
#include <math.h>

#include "StorageTrait.h"
#include "TransformationTrait.h"

#include "Matrix.h"
#include "Transform.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Forward Declarations

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
class Vector;

template <typename T,
		  typename TransformTrait>
class HomogeneousCoordinate;

template<typename T>
class Vector1x3;

template<typename T>
class Vector3x1;

template <typename T>
class Vector4x1;

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
class Matrix;

template <typename T,
		  typename TransformTrait>
class HomogeneousTransform;


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Vector Template Class Declaration

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait=Storage2Dim>
class Vector
{
public:
	typename StorageTrait<T,Rows,Columns>::SimpleArray m_Storage;

	Vector();
	Vector(const Vector<T,Rows,Columns,StorageTrait>& vector);
	Vector(const Matrix<T,Rows,Columns,StorageTrait>& matrix);

	//Specialized Conversion Constructor
	template <typename ComponentTrait>
	Vector(const HomogeneousCoordinate<T,ComponentTrait>& hc);

	virtual ~Vector();

	Vector<T,Rows,Columns,StorageTrait>& operator=(const Vector<T,Rows,Columns,StorageTrait>& vector);

	Vector(const T& f1, const T& f2, const T& f3);
	Vector(const T& f1, const T& f2, const T& f3, const T& f4);

	void Set(const T& f1, const T& f2, const T& f3);
	void Set(const T& f1, const T& f2, const T& f3, const T& f4);

	Vector<T,Rows,Columns,StorageTrait>& operator+=(Vector<T,Rows,Columns,StorageTrait> v);
	Vector<T,Rows,Columns,StorageTrait>& operator-=(Vector<T,Rows,Columns,StorageTrait> v);
	Vector<T,Rows,Columns,StorageTrait>& operator-();


	Vector<T,Rows,Columns,StorageTrait>& operator*=(float s);
	Vector<T,Rows,Columns,StorageTrait>& operator/=(float s)
	{
		for(int i(0);i<Rows;++i)
			for(int j(0);j<Columns;++j) m_Storage.M[i][j]/=s;
		return *this;
	}

	void SetZero()
	{
		for(int i(0);i<Rows;++i) 
			for(int j(0);j<Columns;++j)
				m_Storage.M[i][j]=T();
	}

	T Magnitude()
	{
		T sum=T();
		for(int i(0);i<Rows;++i) for(int j(0);j<Columns;++j)
		{
			sum+=(m_Storage.M[i][j]*m_Storage.M[i][j]);
		}

		return (sqrt(sum));
	}

	void Normalize()
	{
		T magnitude(Magnitude());
		const T t(0.0001f);

		if(magnitude<=t) magnitude=1.0f;
		for(int i(0);i<Rows;++i) for(int j(0);j<Columns;++j)
		{
			m_Storage.M[i][j]/=magnitude;
			if(fabs(m_Storage.M[i][j])<t) m_Storage.M[i][j]=0.0f;
		}
	}

	static float Dot(const Vector<T,Rows,Columns,StorageTrait>& u, const Vector<T,Rows,Columns,StorageTrait>& v);

	void RoundOff()
	{
		for(int i(0);i<Rows;++i) 
		for (int j(0);j<Columns;++j) if (m_Storage.M[i][j]<0.0001f) m_Storage.M[i][j]=0.0f;
	}
};

template<typename T>
class Vector1x3 : public Vector<T,1,3,Storage2Dim>
{
public:
	T& x; T& y; T& z;

	Vector1x3() :
	x(m_Storage.M[0][0]),
	y(m_Storage.M[0][1]),
	z(m_Storage.M[0][2]) 
	{
		//m_Storage.M has already been initialized by the 
		//Vector<T,1,3,Storage2Dim> constructor
	}

	Vector1x3(T xx, T yy, T zz) :
	x(m_Storage.M[0][0]),
	y(m_Storage.M[0][1]),
	z(m_Storage.M[0][2]), Vector<T,1,3,Storage2Dim>(xx,yy,zz)
	{
		assert(x==xx);
		assert(y==yy);
		assert(z==zz);
	}

	//Copy constructor for Base
	Vector1x3(const Vector<T,1,3,Storage2Dim>& rv) :
	x(m_Storage.M[0][0]),
	y(m_Storage.M[0][1]),
	z(m_Storage.M[0][2]), Vector<T,1,3,Storage2Dim>(rv)
	{
		assert(m_Storage.M[0][0]==rv.m_Storage.M[0][0]);
		assert(m_Storage.M[0][1]==rv.m_Storage.M[0][1]);
		assert(m_Storage.M[0][2]==rv.m_Storage.M[0][2]);
	}

	Vector1x3(const Vector1x3<T>& rv) :
	x(m_Storage.M[0][0]),
	y(m_Storage.M[0][1]),
	z(m_Storage.M[0][2]), Vector<T,1,3,Storage2Dim>(rv.x,rv.y,rv.z)
	{
		assert(x==rv.x);
		assert(y==rv.y);
		assert(z==rv.z);
	}

	//Equal operator for Base
	Vector1x3<T>& operator=(const Vector<T,1,3,Storage2Dim>& rv)
	{
		if(this==&rv) return *this;
		
		Vector<T,1,3,Storage2Dim>::operator=(rv);

		assert(rv==*this);

		return *this;
	}

	Vector1x3<T>& operator=(const Vector1x3<T>& rv)
	{
		if(this==&rv) return *this;

		Vector<T,1,3,Storage2Dim>::operator=(rv);

		assert(rv==*this);

		return *this;
	}
};



template<typename T>
class Vector3x1 : public Vector<T,3,1,Storage2Dim>
{
public:
	T& x; T& y; T& z;

	Vector3x1() :
	x(m_Storage.M[0][0]),
	y(m_Storage.M[1][0]),
	z(m_Storage.M[2][0])
	{
		//m_Storage.M has already been initialized by the 
		//Vector<T,3,1,Storage2Dim> constructor
	}

	Vector3x1(T xx, T yy, T zz) :
	x(m_Storage.M[0][0]),
	y(m_Storage.M[1][0]),
	z(m_Storage.M[2][0]), Vector<T,3,1,Storage2Dim>(xx,yy,zz)
	{
		assert(x==xx);
		assert(y==yy);
		assert(z==zz);
	}

	Vector3x1(const Vector3x1<T>& rv) :
	x(m_Storage.M[0][0]),
	y(m_Storage.M[1][0]),
	z(m_Storage.M[2][0]), Vector<T,3,1,Storage2Dim>(rv.x,rv.y,rv.z)
	{
		assert(x==rv.x);
		assert(y==rv.y);
		assert(z==rv.z);
	}

	Vector3x1(const Vector<T,3,1,Storage2Dim>& rv) :
	x(m_Storage.M[0][0]),
	y(m_Storage.M[1][0]),
	z(m_Storage.M[2][0]), Vector<T,3,1,Storage2Dim>(rv)
	{
		assert(m_Storage.M[0][0]==rv.m_Storage.M[0][0]);
		assert(m_Storage.M[1][0]==rv.m_Storage.M[1][0]);
		assert(m_Storage.M[2][0]==rv.m_Storage.M[2][0]);
	}

	Vector3x1(const Vector4x1<T>& hv) :
	x(m_Storage.M[0][0]),
	y(m_Storage.M[1][0]),
	z(m_Storage.M[2][0]), Vector<T,3,1,Storage2Dim>(hv.x/hv.w,hv.y/hv.w,hv.z/hv.w)
	{
	}

	Vector3x1& operator=(const Vector<T,3,1,Storage2Dim>& rv)
	{
		if(this==&rv) return *this;
		
		//Use the base class to copy
		Vector<T,3,1,Storage2Dim>::operator=(rv);

		assert(rv==*this);

		return *this;
	}

	Vector3x1& operator=(const Vector3x1<T>& rv)
	{
		if(this==&rv) return *this;

		//Use the base class to copy
		Vector<T,3,1,Storage2Dim>::operator=(rv);

		assert(rv==*this);

		return *this;
	}
};



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Helper Function Template Declarations (Useful for solving equations)

//V+V
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator+(const Vector<T,Rows,Columns,StorageTrait>& v1, const Vector<T,Rows,Columns,StorageTrait>& v2);

//V*s
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator*(const Vector<T,Rows,Columns,StorageTrait>& v, const float& s);

//s*V
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator*(const float& s, const Vector<T,Rows,Columns,StorageTrait>& v);

//V-V
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator-(const Vector<T,Rows,Columns,StorageTrait>& v1, const Vector<T,Rows,Columns,StorageTrait>& v2);


//V==V
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
bool operator==(const Vector<T,Rows,Columns,StorageTrait> &vector1, const Vector<T,Rows,Columns,StorageTrait> &vector2);

//V!=V
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
bool operator!=(const Vector<T,Rows,Columns,StorageTrait> &vector1, const Vector<T,Rows,Columns,StorageTrait> &vector2);

//V*M
template <typename T,
		  int Rows,
		  int Columns,
		  int CommonSize,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator*(const Vector<T,Rows,CommonSize,StorageTrait>& v, const Matrix<T,CommonSize,Columns,StorageTrait>& m);

//M*V
template <typename T,
		  int Rows,
		  int Columns,
		  int CommonSize,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator*(const Matrix<T,Rows,CommonSize,StorageTrait>& m, const Vector<T,CommonSize,Columns,StorageTrait>& v);

//V/s
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator/(const Vector<T,Rows,Columns,StorageTrait>& v, const float& s);


template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,3,1,StorageTrait> Cross(Vector<T,3,1,StorageTrait> u, Vector<T,3,1,StorageTrait> v);

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,3,1,StorageTrait> Projection(Vector<T,3,1,StorageTrait> P, Vector<T,3,1,StorageTrait> Q);

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,3,1,StorageTrait> Perpendicular(Vector<T,3,1,StorageTrait> P, Vector<T,3,1,StorageTrait> Q);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Vector Member Template Definitions

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait>::Vector()
{
	for(int i(0);i<Rows;++i)
		for(int j(0);j<Columns;++j) 
			m_Storage.M[i][j]=T();
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait>::Vector(const Vector<T,Rows,Columns,StorageTrait>& vector)
{
	for(int i(0);i<Rows;++i)
		for(int j(0);j<Columns;++j) 
			m_Storage.M[i][j]=vector.m_Storage.M[i][j];
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait>::Vector(const Matrix<T,Rows,Columns,StorageTrait>& matrix)
{
	for(int i(0);i<Rows;++i)
		for(int j(0);j<Columns;++j) 
			m_Storage.M[i][j]=matrix.m_Storage.M[i][j];
}



template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
template <typename ComponentTrait>
Vector<T,Rows,Columns,StorageTrait>::Vector(const HomogeneousCoordinate<T,ComponentTrait>& hc)
{
	for(int i(0);i<Rows;++i)
		for(int j(0);j<Columns;++j) 
			m_Storage.M[i][j]=hc.m_Storage.M[j][0]/hc.m_Storage.M[3][0];

}



template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait>::Vector(const T& f1, const T& f2, const T& f3)
{
	if(Rows>=3 && Columns>=1)
	{
		m_Storage.M[0][0]=f1;
		m_Storage.M[1][0]=f2;
		m_Storage.M[2][0]=f3;
	}
	else if (Rows>=1 && Columns>=3)
	{
		m_Storage.M[0][0]=f1;
		m_Storage.M[0][1]=f2;
		m_Storage.M[0][2]=f3;
	}
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait>::Vector(const T& f1, const T& f2, const T& f3, const T& f4)
{
	if(Rows>=4 && Columns>=1)
	{
		m_Storage.M[0][0]=f1;
		m_Storage.M[1][0]=f2;
		m_Storage.M[2][0]=f3;
		m_Storage.M[3][0]=f4;
	}
	else if (Rows>=1 && Columns>=4)
	{
		m_Storage.M[0][0]=f1;
		m_Storage.M[0][1]=f2;
		m_Storage.M[0][2]=f3;
		m_Storage.M[0][3]=f3;
	}
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait>::~Vector()
{
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait>& Vector<T,Rows,Columns,StorageTrait>::operator=(const Vector<T,Rows,Columns,StorageTrait>& vector)
{
	if(&vector==this) return *this;

	for(int i(0);i<Rows;++i) 
		for(int j(0);j<Columns;++j)
			m_Storage.M[i][j]=vector.m_Storage.M[i][j];
	
	assert(vector==*this);

	return *this;
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
void Vector<T,Rows,Columns,StorageTrait>::Set(const T& f1, const T& f2, const T& f3)
{
	if(Rows>=3 && Columns>=1)
	{
		m_Storage.M[0][0]=f1;
		m_Storage.M[1][0]=f2;
		m_Storage.M[2][0]=f3;
	}
	else if (Rows>=1 && Columns>=3)
	{
		m_Storage.M[0][0]=f1;
		m_Storage.M[0][1]=f2;
		m_Storage.M[0][2]=f3;
	}
	else
	{
		//Not enough Rows or Columns
		assert(0);
	}
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
void Vector<T,Rows,Columns,StorageTrait>::Set(const T& f1, const T& f2, const T& f3, const T& f4)
{
	if(Rows>=4 && Columns>=1)
	{
		m_Storage.M[0][0]=f1;
		m_Storage.M[1][0]=f2;
		m_Storage.M[2][0]=f3;
		m_Storage.M[3][0]=f4;
	}
	else if (Rows>=1 && Columns>=4)
	{
		m_Storage.M[0][0]=f1;
		m_Storage.M[0][1]=f2;
		m_Storage.M[0][2]=f3;
		m_Storage.M[0][3]=f3;
	}
	else
	{
		//Not enough Rows or Columns
		assert(0);
	}
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait>& Vector<T,Rows,Columns,StorageTrait>::operator+=(Vector<T,Rows,Columns,StorageTrait> v)
{
	for(int i(0);i<Rows;++i) 
		for(int j(0);j<Columns;++j)
			m_Storage.M[i][j]+=v.m_Storage.M[i][j];
	return *this;
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait>& Vector<T,Rows,Columns,StorageTrait>::operator-=(Vector<T,Rows,Columns,StorageTrait> v)
{
	for(int i(0);i<Rows;++i) 
		for(int j(0);j<Columns;++j)
			m_Storage.M[i][j]-=v.m_Storage.M[i][j];
	return *this;
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait>& Vector<T,Rows,Columns,StorageTrait>::operator-()
{
	for(int i(0);i<Rows;++i) 
		for(int j(0);j<Columns;++j)
			m_Storage.M[i][j]=-m_Storage.M[i][j];

	return *this;
}



template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait>& Vector<T,Rows,Columns,StorageTrait>::operator*=(float s)
{
	for(int i(0);i<Rows;++i)
		for(int j(0);j<Columns;++j)
			m_Storage.M[i][j]*=s;

	return *this;
}

///
///	Helper Functions
///

//V-V
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator-(const Vector<T,Rows,Columns,StorageTrait>& v1, const Vector<T,Rows,Columns,StorageTrait>& v2)
{
	Vector<T,Rows,Columns,StorageTrait> ret;

	for(int i(0);i<Rows;++i)
		for(int j(0);j<Columns;++j)
			ret.m_Storage.M[i][j]=((v1.m_Storage.M[i][j])-(v2.m_Storage.M[i][j]));

	return ret;
}

//V+V
template <typename T,
		  template <typename,int,int> class StorageTrait>
Vector<T,3,1,StorageTrait> operator+(const Vector<T,3,1,StorageTrait>& v1, const Vector<T,3,1,StorageTrait>& v2)
{
	return Vector<T,3,1,StorageTrait>(v1.m_Storage.M[0][0]+v2.m_Storage.M[0][0],
									  v1.m_Storage.M[1][0]+v2.m_Storage.M[1][0],
									  v1.m_Storage.M[2][0]+v2.m_Storage.M[2][0]);
}

//V+V
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator+(const Vector<T,Rows,Columns,StorageTrait>& v1, const Vector<T,Rows,Columns,StorageTrait>& v2)
{
	Vector<T,Rows,Columns,StorageTrait> ret;

	for(int i(0);i<Rows;++i)
		for(int j(0);j<Columns;++j)
			ret.m_Storage.M[i][j]=v1.m_Storage.M[i][j]+v2.m_Storage.M[i][j];


	return ret;
}

//V*s
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator*(const Vector<T,Rows,Columns,StorageTrait>& v, const float& s)
{
	Vector<T,Rows,Columns,StorageTrait> ret;
	for(int i(0);i<Rows;++i) 
		for(int j(0);j<Columns;++j)
			ret.m_Storage.M[i][j]=v.m_Storage.M[i][j]*s;

	return ret;
}

//s*V
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator*(const float& s, const Vector<T,Rows,Columns,StorageTrait>& v )
{
	Vector<T,Rows,Columns,StorageTrait> ret;
	for(int i(0);i<Rows;++i)
		for(int j(0);j<Columns;++j)
			ret.m_Storage.M[i][j]=v.m_Storage.M[i][j]*s;

	return ret;
}

//V==V
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
bool operator==(const Vector<T,Rows,Columns,StorageTrait> &vector1, const Vector<T,Rows,Columns,StorageTrait> &vector2)
{
	//TODO: Adjust for Rounding errors

	bool flag=true;
	T f(0.001f);

	for(int i(0);i<Rows;++i) for(int j(0);j<Columns;++j)
	{
		if(vector1.m_Storage.M[i][j]!=vector2.m_Storage.M[i][j])
		{
			flag=false;

			if (fabs(vector1.m_Storage.M[i][j]-vector2.m_Storage.M[i][j])<=f)
			{
				flag=true;
				continue;
			}

			break;
		}
	}
	return flag;
}

//V!=V
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
bool operator!=(const Vector<T,Rows,Columns,StorageTrait> &vector1, const Vector<T,Rows,Columns,StorageTrait> &vector2)
{
	
	bool flag=true;
	T f=0.001f;

	for(int i(0);i<Rows;++i) for(int j(0);j<Columns;++j)
	{
		if(vector1.m_Storage.M[i][j]==vector2.m_Storage.M[i][j])
		{
			flag=false;
			break;
		}
		
		//Adjust for Rounding errors
		if (fabs(vector1.m_Storage.M[i][j]-vector2.m_Storage.M[i][j])<=f)
		{
			flag=false;
			break;
		}
	}
	return flag;
}


template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
float Vector<T,Rows,Columns,StorageTrait>::Dot(const Vector<T,Rows,Columns,StorageTrait>& u, const Vector<T,Rows,Columns,StorageTrait>& v)
{
	T sum=T();

	for(int i(0);i<Rows;++i)
		for(int j(0);j<Columns;++j)
			sum+=(u.m_Storage.M[i][j]*v.m_Storage.M[i][j]);

	return sum;
}


template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,3,1,StorageTrait> Cross(Vector<T,3,1,StorageTrait> u, Vector<T,3,1,StorageTrait> v)
{
	return Vector<T,3,1,StorageTrait>(
		u.m_Storage.M[1][0]*v.m_Storage.M[2][0]-u.m_Storage.M[2][0]*v.m_Storage.M[1][0],
		-u.m_Storage.M[0][0]*v.m_Storage.M[2][0]+u.m_Storage.M[2][0]*v.m_Storage.M[0][0],
		u.m_Storage.M[0][0]*v.m_Storage.M[1][0]-u.m_Storage.M[1][0]*v.m_Storage.M[0][0]);				
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,3,1,StorageTrait> Projection(Vector<T,3,1,StorageTrait> P, Vector<T,3,1,StorageTrait> Q)
{
	T q=1/pow(Q.Magnitude(),2);
	Vector<T,3,1> ret((Dot<float,3,1>(P,Q))*q*Q);

	return ret;
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,3,1,StorageTrait> Perpendicular(Vector<T,3,1,StorageTrait> P, Vector<T,3,1,StorageTrait> Q)
{
	T q=1/pow(Q.Magnitude(),2);

	Vector<T,3,1> ret(P-((Dot<float,3,1>(P,Q))*q*Q));

	return ret;
}

//V*M
template <typename T,
		  int Rows,
		  int Columns,
		  int CommonSize,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator*(const Vector<T,Rows,CommonSize,StorageTrait>& v, const Matrix<T,CommonSize,Columns,StorageTrait>& m)
{
	Matrix<T,Rows,CommonSize,StorageTrait> t(v);
	Matrix<T,Rows,Columns,StorageTrait> s(t*m);
	Vector<T,Rows,Columns,StorageTrait> ret(s);

	return ret;
}

//M*V
template <typename T,
		  int Rows,
		  int Columns,
		  int CommonSize,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator*(const Matrix<T,Rows,CommonSize,StorageTrait>& m,const Vector<T,CommonSize,Columns,StorageTrait>& v)
{
	Matrix<T,CommonSize,Columns> t(v);
	Matrix<T,Rows,Columns> s(m*t);
	Vector<T,Rows,Columns> ret(s);

	return ret;
}

//V/s
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Vector<T,Rows,Columns,StorageTrait> operator/(const Vector<T,Rows,Columns,StorageTrait>& v, const float& s)
{
	Vector<T,Rows,Columns> ret;
	for (int i(0);i<Rows; ++i)
		for(int j(0);j<Columns;++j)
			ret.m_Storage.M[i][j]=v.m_Storage.M[i][j]/s;

	return ret;
}



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// Homogeneous Coordinate (4 Tuple x,y,z,w )
template <typename T,
		  typename TransformTrait=Real3>
class HomogeneousCoordinate : public Vector<T,4,1,Storage2Dim>
{
public:

	T& x; 
	T& y; 
	T& z; 
	T& w;

	HomogeneousCoordinate() : Vector<T,4,1,Storage2Dim>(0,0,0,0),	//Point
							x(m_Storage.M[0][0]),
							y(m_Storage.M[1][0]),
							z(m_Storage.M[2][0]),
							w(m_Storage.M[3][0])
	{
	}

	HomogeneousCoordinate(const T& xx, const T& yy, const T& zz, const T& ww) : Vector<T,4,1,Storage2Dim>(xx,yy,zz,ww),	//Point
							x(m_Storage.M[0][0]),
							y(m_Storage.M[1][0]),
							z(m_Storage.M[2][0]),
							w(m_Storage.M[3][0])
	{
		//w should be zero
		//assert(w==0);
	}

	HomogeneousCoordinate(const HomogeneousCoordinate<T,TransformTrait>& hc);

	//Copy constuctor for Base
	HomogeneousCoordinate(const Vector<T,4,1,Storage2Dim>& v) : Vector<T,4,1,Storage2Dim>(v),	
							x(m_Storage.M[0][0]),
							y(m_Storage.M[1][0]),
							z(m_Storage.M[2][0]),
							w(m_Storage.M[3][0])
	{
	}

	//Equal operator for Base
	HomogeneousCoordinate<T,TransformTrait>& operator=(const Vector<T,4,1,Storage2Dim>& v)
	{
		if(this==&v) return *this;

		Vector<T,4,1,Storage2Dim>::operator=(v);

		return *this;
	}

	HomogeneousCoordinate(const Vector<T,3,1,Storage2Dim>& vector);
	HomogeneousCoordinate(const Vector<T,1,3,Storage2Dim>& vector);

	
	
	~HomogeneousCoordinate();

	HomogeneousCoordinate<T,TransformTrait>& operator=(const HomogeneousCoordinate<T,TransformTrait>& hc);
};


//HC*HT
template <typename T,
		  typename TransformTrait>
HomogeneousCoordinate<T,TransformTrait> operator*(const HomogeneousCoordinate<T,TransformTrait>& hc,const HomogeneousTransform<T,TransformTrait>& ht);



template <typename T,
		  typename TransformTrait>
HomogeneousCoordinate<T,TransformTrait>::HomogeneousCoordinate(const HomogeneousCoordinate<T,TransformTrait>& hc) :
							Vector<T,4,1,Storage2Dim>(hc.m_Storage.M[0][0],
													  hc.m_Storage.M[1][0],
													  hc.m_Storage.M[2][0],
													  hc.m_Storage.M[3][0]),
							x(m_Storage.M[0][0]),
							y(m_Storage.M[1][0]),
							z(m_Storage.M[2][0]),
							w(m_Storage.M[3][0])
{
}

template <typename T,
		  typename TransformTrait>
HomogeneousCoordinate<T,TransformTrait>::HomogeneousCoordinate(const Vector<T,3,1,Storage2Dim>& vector) :
							Vector<T,4,1,Storage2Dim>(vector.m_Storage.M[0][0],
													  vector.m_Storage.M[1][0],
													  vector.m_Storage.M[2][0],
													  T()),	//Point
							x(m_Storage.M[0][0]),
							y(m_Storage.M[1][0]),
							z(m_Storage.M[2][0]),
							w(m_Storage.M[3][0])
{
		//w should be zero
		assert(w==0);
}

template <typename T,
		  typename TransformTrait>
HomogeneousCoordinate<T,TransformTrait>::HomogeneousCoordinate(const Vector<T,1,3,Storage2Dim>& vector) :
							Vector<T,4,1,Storage2Dim>(vector.m_Storage.M[0][0],
													  vector.m_Storage.M[0][1],
													  vector.m_Storage.M[0][2],
													  T()),	//Point
							x(m_Storage.M[0][0]),
							y(m_Storage.M[1][0]),
							z(m_Storage.M[2][0]),
							w(m_Storage.M[3][0])
{
		//w should be zero
		assert(w==0);
}

template <typename T,
		  typename TransformTrait>
HomogeneousCoordinate<T,TransformTrait>::~HomogeneousCoordinate()
{
}

template <typename T,
		  typename TransformTrait>
HomogeneousCoordinate<T,TransformTrait>& HomogeneousCoordinate<T,TransformTrait>::operator=(const HomogeneousCoordinate<T,TransformTrait>& hc)
{
	if(this==&hc) return *this;
	
	Vector<T,4,1,Storage2Dim>::operator=(hc);

	return *this;
}

//HT*HC
template <typename T,
		  typename TransformTrait>
HomogeneousCoordinate<T,TransformTrait> operator*(const HomogeneousTransform<T,TransformTrait>& ht,const HomogeneousCoordinate<T,TransformTrait>& hc)
{
	typename TransformTrait<T>::ColumnVector V(hc);

	//TODO: Check this equation!
	HomogeneousCoordinate<T> result=(((ht.m_M*V)+(hc.w*ht.m_T)/hc.w));

	return result;
}

//


template <typename T>
class Vector4x1 : public Vector<T,4,1,Storage2Dim>
{
public:
	T& x; T& y; T& z; T& w; 

	Vector4x1() : 
		x(m_Storage.M[0][0]),
		y(m_Storage.M[1][0]),
		z(m_Storage.M[2][0]),
		w(m_Storage.M[3][0])
	{
		w=1.0f;	//Direction
	}

	Vector4x1(T xx, T yy, T zz, T ww) : 
		x(m_Storage.M[0][0]),
		y(m_Storage.M[1][0]),
		z(m_Storage.M[2][0]),
		w(m_Storage.M[3][0]), Vector<T,4,1,Storage2Dim>(xx,yy,zz,ww)
	{
	}

	Vector4x1(const Vector4x1<T>& v) : 
		x(m_Storage.M[0][0]),
		y(m_Storage.M[1][0]),
		z(m_Storage.M[2][0]),
		w(m_Storage.M[3][0]), Vector<T,4,1,Storage2Dim>(v.x,v.y,v.z,v.w)
	{
	}

	Vector4x1(const Vector3x1<T>& v) : 
		x(m_Storage.M[0][0]),
		y(m_Storage.M[1][0]),
		z(m_Storage.M[2][0]),
		w(m_Storage.M[3][0]), Vector<T,4,1,Storage2Dim>(v.x,v.y,v.z,1.0f)
	{
	}

	~Vector4x1()
	{
	}

	Vector4x1& operator=(const Vector4x1<T>& v)
	{
		if(this==&v) return *this;
		Vector<T,4,1,Storage2Dim>::operator=(v);

		x=v.x;
		y=v.y;
		z=v.z;
		w=v.w;

		return *this;
	}
};

template <typename T>
Vector4x1<T> operator+(const Vector4x1<T>& v1,const Vector4x1<T>& v2);

template <typename T>
Vector4x1<T> operator+(const Vector4x1<T>& v1,const Vector4x1<T>& v2)
{
	//TODO: Check this equation!
	return Vector4x1<T>((v1.x+v2.x),(v1.y+v2.y),(v1.z+v2.z),(1));
}

//HT*Vector4x1 Helper Function
template <typename T,
		  typename TransformTrait>
Vector4x1<T> operator*(const HomogeneousTransform<T,TransformTrait>& ht,const Vector4x1<T>& v);

//HT*Vector4x1 Helper Function
template <typename T,
		  typename TransformTrait>
Vector4x1<T> operator*(const HomogeneousTransform<T,TransformTrait>& ht,const Vector4x1<T>& homogenousvector)
{
	//w must be 1
	assert(homogenousvector.w==1);

	typename TransformTrait::ColumnVector cv(homogenousvector);

	//TODO: Check this equation!
	return Vector4x1<T>((ht.m_M*cv)+ht.m_T);
}