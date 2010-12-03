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

#include "StorageTrait.h"
#include "Vector.h"
#include "Transform.h"

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

template <typename T>
class Quaternion;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Matrix Template Class Declaration

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait=Storage2Dim>
class Matrix
{
public:
	typename StorageTrait<T,Rows,Columns>::SimpleArray m_Storage;

	Matrix();

	Matrix(T r1c1,T r1c2,T r1c3,
		   T r2c1,T r2c2,T r2c3,
		   T r3c1,T r3c2,T r3c3);

	Matrix(const Matrix<float,Rows,Columns,StorageTrait>& matrix);
	Matrix(const Vector<float,Rows,Columns,StorageTrait>& vector);
	~Matrix();

	Matrix<T,Rows,Columns,StorageTrait>& operator=(const Matrix<T,Rows,Columns,StorageTrait>& matrix);
	Matrix<T,Rows,Columns,StorageTrait>& operator*= (T s);
	Matrix<T,Rows,Columns,StorageTrait>& operator/= (T s);
	Matrix<T,Rows,Columns,StorageTrait> operator-();

	T GetDeterminant();
	void Invert();
	void SetIdentity();
	void SetZero();
	void Transpose();

	//Fix Rounding errors
	void RoundOff()
	{
		for(int i(0);i<Rows;++i) 
		for (int j(0);j<Columns;++j) if (m_Storage.M[i][j]<0.0001f) m_Storage.M[i][j]=0.0f;
	}
};


//
template<typename T>
class Matrix3x3 : public Matrix<T,3,3,Storage2Dim>
{
public:
	T& m11; T& m12; T& m13;
	T& m21; T& m22; T& m23;
	T& m31; T& m32; T& m33;

	Matrix3x3() :
		m11(m_Storage.M[0][0]),m12(m_Storage.M[0][1]),m13(m_Storage.M[0][2]),
		m21(m_Storage.M[1][0]),m22(m_Storage.M[1][1]),m23(m_Storage.M[1][2]),
		m31(m_Storage.M[2][0]),m32(m_Storage.M[2][1]),m33(m_Storage.M[2][2])
	{
	}

	Matrix3x3(
		float am11, float am12, float am13,
		float am21, float am22, float am23,
		float am31, float am32, float am33) :
		m11(m_Storage.M[0][0]),m12(m_Storage.M[0][1]),m13(m_Storage.M[0][2]),
		m21(m_Storage.M[1][0]),m22(m_Storage.M[1][1]),m23(m_Storage.M[1][2]),
		m31(m_Storage.M[2][0]),m32(m_Storage.M[2][1]),m33(m_Storage.M[2][2])
	{
		m11=(am11), m12=(am12), m13=(am13);
		m21=(am21), m22=(am22), m23=(am23);
		m31=(am31), m32=(am32), m33=(am33);
	}

	Matrix3x3(const Quaternion<T>& q) :
		m11(m_Storage.M[0][0]),m12(m_Storage.M[0][1]),m13(m_Storage.M[0][2]),
		m21(m_Storage.M[1][0]),m22(m_Storage.M[1][1]),m23(m_Storage.M[1][2]),
		m31(m_Storage.M[2][0]),m32(m_Storage.M[2][1]),m33(m_Storage.M[2][2])
	{
		//Copyright (C) 1998,1999,2000 by Jorrit Tyberghein
		//Largely rewritten by Ivan Avramovic <ivan@avramovic.com>
		//Crystal Space

		float rx, ry, rz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

		// calculate coefficients
		x2 = q.x + q.x;
		y2 = q.y + q.y;
		z2 = q.z + q.z;
		xx = q.x * x2;   xy = q.x * y2;   xz = q.x * z2;
		yy = q.y * y2;   yz = q.y * z2;   zz = q.z * z2;
		rx = q.w * x2;   ry = q.w * y2;   rz = q.w * z2;

		m_Storage.M[0][0] = 1.0f - (yy + zz);
		m_Storage.M[0][1] = xy - rz;
		m_Storage.M[0][2] = xz + ry;

		m_Storage.M[1][0] = xy + rz;
		m_Storage.M[1][1] = 1.0f - (xx + zz);
		m_Storage.M[1][2] = yz - rx;

		m_Storage.M[2][0] = xz - ry;
		m_Storage.M[2][1] = yz + rx;
		m_Storage.M[2][2] = 1.0f - (xx + yy);
	}

	Matrix3x3<T>& operator=(const Matrix3x3<T>& matrix);
	Matrix3x3<T>& operator=(const Matrix<T,3,3,Storage2Dim>& matrix);
};


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Helper Function Template Declarations (Useful for solving equations)

//M*f
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait> operator * (const Matrix<T,Rows,Columns,StorageTrait> &matrix, T f);

//f*M
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait> operator * (T f, const Matrix<T,Rows,Columns,StorageTrait> &matrix);

//M/f
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait> operator/ (const Matrix<T,Rows,Columns,StorageTrait> &matrix, T f);

//M+M
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait> operator+ (const Matrix<T,Rows,Columns,StorageTrait> &matrix1, const Matrix<T,Rows,Columns,StorageTrait> &matrix2);

//M-M
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait> operator- (const Matrix<T,Rows,Columns,StorageTrait> &matrix1, const Matrix<T,Rows,Columns,StorageTrait> &matrix2);

//M*M
template <typename T,
		  int Rows1,
		  int Columns1,
		  int Rows2,
		  int Columns2,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows1,Columns2,StorageTrait> operator * (const Matrix<T,Rows1,Columns1,StorageTrait> &matrix1, const Matrix<T,Rows2,Columns2,StorageTrait> &matrix2);

//M==M
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
bool operator== (const Matrix<T,Rows,Columns,StorageTrait> &matrix1, const Matrix<T,Rows,Columns,StorageTrait> &matrix2);

//M!=M
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
bool operator!= (const Matrix<T,Rows,Columns,StorageTrait> &matrix1, const Matrix<T,Rows,Columns,StorageTrait> &matrix2);


template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Columns,Rows,StorageTrait> Transpose(const Matrix<T,Rows,Columns,StorageTrait>& matrix);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//Matrix Member Template Definitions

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait>::Matrix()
{
	for(int i(0);i<Rows;++i) 
		for (int j(0);j<Columns;++j) 
			if(i!=j) m_Storage.M[i][j]=T(); else m_Storage.M[i][j]=1;
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait>::Matrix(T r1c1,T r1c2,T r1c3,
											T r2c1,T r2c2,T r2c3,
											T r3c1,T r3c2,T r3c3)
{
	m_Storage.M[0][0]=(r1c1),m_Storage.M[0][1]=(r1c2),m_Storage.M[0][2]=(r1c3),
	m_Storage.M[1][0]=(r2c1),m_Storage.M[1][1]=(r2c2),m_Storage.M[1][2]=(r2c3),
	m_Storage.M[2][0]=(r3c1),m_Storage.M[2][1]=(r3c2),m_Storage.M[2][2]=(r3c3);
}


template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait>::Matrix(const Matrix<float,Rows,Columns,StorageTrait>& matrix)
{
	for(int i(0);i<Rows;++i) for (int j(0);j<Columns;++j) m_Storage.M[i][j]=matrix.m_Storage.M[i][j];
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait>::Matrix(const Vector<float,Rows,Columns,StorageTrait>& vector)
{
	for(int i(0);i<Rows;++i)
		for(int j(0);j<Columns;++j)
			m_Storage.M[i][j]=vector.m_Storage.M[i][j];
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait>::~Matrix()
{
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait>& Matrix<T,Rows,Columns,StorageTrait>::operator=(const Matrix<T,Rows,Columns,StorageTrait>& matrix)
{
	if(&matrix==this) return *this;

	for(int i(0);i<Rows;++i) for (int j(0);j<Columns;++j) m_Storage.M[i][j]=matrix.m_Storage.M[i][j];

	assert(matrix==*this);

	return *this;
}

template<typename T>
Matrix3x3<T>& Matrix3x3<T>::operator=(const Matrix3x3<T>& matrix)
{
	if(&matrix==this) return *this;

	Matrix<T,3,3,Storage2Dim>::operator=(matrix);

	assert(matrix==*this);

	return *this;
}

template<typename T>
Matrix3x3<T>& Matrix3x3<T>::operator=(const Matrix<T,3,3,Storage2Dim>& matrix)
{
	if(&matrix==this) return *this;

	Matrix<T,3,3,Storage2Dim>::operator=(matrix);

	assert(matrix==*this);

	return *this;
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
T Matrix<T,Rows,Columns,StorageTrait>::GetDeterminant()
{
	T d=T();

	if(Rows==2 && Columns==2)
	{
		d=(m_Storage.M[0][0]*m_Storage.M[1][1])-(m_Storage.M[0][1]*m_Storage.M[1][0]);
	}
	else if(Rows==3 && Columns==3)
	{
		d=(m_Storage.M[0][0]*((m_Storage.M[1][1]*m_Storage.M[2][2])-(m_Storage.M[1][2]*m_Storage.M[2][1])))-
		  (m_Storage.M[0][1]*((m_Storage.M[1][0]*m_Storage.M[2][2])-(m_Storage.M[1][2]*m_Storage.M[2][0])))+
		  (m_Storage.M[0][2]*((m_Storage.M[1][0]*m_Storage.M[2][1])-(m_Storage.M[1][1]*m_Storage.M[2][0])));
	}
	else
	{
		//TODO: Handle nXm Matrix
		assert(0);
	}
	return d;
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
void Matrix<T,Rows,Columns,StorageTrait>::SetIdentity()
{
	for(int i(0);i<Rows;++i) 
		for (int j(0);j<Columns;++j) 
			if(i!=j) m_Storage.M[i][j]=T(); else m_Storage.M[i][j]=1;
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
void Matrix<T,Rows,Columns,StorageTrait>::Transpose()
{
	Matrix<T,Rows,Columns> temp(*this);
	for(int i(0);i<Rows;++i) 
		for (int j(0);j<Columns;++j)
			m_Storage.M[i][j]=temp.m_Storage.M[j][i];
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
void Matrix<T,Rows,Columns,StorageTrait>::SetZero()
{
	for(int i(0);i<Rows;++i) 
		for (int j(0);j<Columns;++j) m_Storage.M[i][j]=T();
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
void Matrix<T,Rows,Columns,StorageTrait>::Invert()
{
	Matrix<T,Rows,Columns> temp;

	T det=GetDeterminant();

	//The Matrix is invertible if and only if the determinant is not zero
	assert(det!=0);

	T d=1/det;

	if(Rows==2 && Columns==2)
	{
		temp.m_Storage.M[0][0]=m_Storage.M[1][1], temp.m_Storage.M[0][1]=-m_Storage.M[0][1];
		temp.m_Storage.M[1][0]=-m_Storage.M[1][0], temp.m_Storage.M[1][1]=m_Storage.M[0][0];
		
		temp*=d;
		*this=temp;
	}
	else if(Rows==3 && Columns==3)
	{
		temp.m_Storage.M[0][0]=(m_Storage.M[1][1]*m_Storage.M[2][2] - m_Storage.M[1][2]*m_Storage.M[2][1]), 
		temp.m_Storage.M[0][1]=(m_Storage.M[0][2]*m_Storage.M[2][1] - m_Storage.M[0][1]*m_Storage.M[2][2]), 
		temp.m_Storage.M[0][2]=(m_Storage.M[0][1]*m_Storage.M[1][2] - m_Storage.M[0][2]*m_Storage.M[1][1]);

        temp.m_Storage.M[1][0]=(m_Storage.M[1][2]*m_Storage.M[2][0] - m_Storage.M[1][0]*m_Storage.M[2][2]), 
		temp.m_Storage.M[1][1]=(m_Storage.M[0][0]*m_Storage.M[2][2] - m_Storage.M[0][2]*m_Storage.M[2][0]), 
		temp.m_Storage.M[1][2]=(m_Storage.M[0][2]*m_Storage.M[1][0] - m_Storage.M[0][0]*m_Storage.M[1][2]);

        temp.m_Storage.M[2][0]=(m_Storage.M[1][0]*m_Storage.M[2][1] - m_Storage.M[1][1]*m_Storage.M[2][0]), 
		temp.m_Storage.M[2][1]=(m_Storage.M[0][1]*m_Storage.M[2][0] - m_Storage.M[0][0]*m_Storage.M[2][1]), 
		temp.m_Storage.M[2][2]=(m_Storage.M[0][0]*m_Storage.M[1][1] - m_Storage.M[0][1]*m_Storage.M[1][0]);
	
		temp*=d;

#ifdef _DEBUG

	Matrix<float,3,3> M1,M2,I,M4,M5;
	M1=*this;	//not inverted
	M2=temp;	//inverted
	I.SetIdentity();
	M4=M1*M2;
	M5=M2*M1;
	
	M4.RoundOff();
	M5.RoundOff();
	assert(M4==I);
	assert(M5==I);
	assert(M4==M5);

#endif _DEBUG

		*this=temp;
	}
	else
	{
		//TODO: Handle nXm Matrix
		assert(0);
	}


}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait>& Matrix<T,Rows,Columns,StorageTrait>::operator*= (T s)
{
	for(int i(0);i<Rows;++i) for (int j(0);j<Columns;++j) m_Storage.M[i][j]*=s;

	return *this;
}


template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
		  Matrix<T,Rows,Columns,StorageTrait>& Matrix<T,Rows,Columns,StorageTrait>::operator/= (T s)
{
	assert(s!=0.0f);

	T inv_s = 1 / s;

	for(int i(0);i<Rows;++i) for (int j(0);j<Columns;++j) m_Storage.M[i][j]=m_Storage.M[i][j]*inv_s;

	return *this;
}

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait> Matrix<T,Rows,Columns,StorageTrait>::operator-()
{
	for(int i(0);i<Rows;++i) for (int j(0);j<Columns;++j) m_Storage.M[i][j]=-m_Storage.M[i][j];
	return *this;
}

///
/// Helper Functions
///

//M*f
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait> operator * (const Matrix<T,Rows,Columns,StorageTrait> &matrix, T f)
{
	Matrix<T,Rows,Columns,StorageTrait> ret;
	for(int i(0);i<Rows;++i) for (int j(0);j<Columns;++j) ret.m_Storage.M[i][j]=matrix.m_Storage.M[i][j]*f;
	
	return ret;
}

//f*M
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait> operator * (T f, const Matrix<T,Rows,Columns,StorageTrait> &matrix)
{
	Matrix<T,Rows,Columns,StorageTrait> ret;
	for(int i(0);i<Rows;++i) for (int j(0);j<Columns;++j) ret.m_Storage.M[i][j]=matrix.m_Storage.M[i][j]*f;
	
	return ret;
}

//M/f
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait> operator/ (const Matrix<T,Rows,Columns,StorageTrait> &matrix, T f)
{
	assert(f!=0.0f);

	T inv_f = 1 / f;

	Matrix<T,Rows,Columns,StorageTrait> ret;
	for(int i(0);i<Rows;++i) for (int j(0);j<Columns;++j) ret.m_Storage.M[i][j]=matrix.m_Storage.M[i][j]*inv_f;

	return ret;
}


//M+M
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait> operator+ (const Matrix<T,Rows,Columns,StorageTrait> &matrix1, const Matrix<T,Rows,Columns,StorageTrait> &matrix2)
{
	Matrix<T,Rows,Columns,StorageTrait> ret;
	for(int i(0);i<Rows;++i) for (int j(0);j<Columns;++j) ret.m_Storage.M[i][j]=matrix1.m_Storage.M[i][j]+matrix2.m_Storage.M[i][j];
	return ret;
}


//M-M
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows,Columns,StorageTrait> operator- (const Matrix<T,Rows,Columns,StorageTrait> &matrix1, const Matrix<T,Rows,Columns,StorageTrait> &matrix2)
{
	Matrix<T,Rows,Columns,StorageTrait> ret;
	for(int i(0);i<Rows;++i) for (int j(0);j<Columns;++j) ret.m_Storage.M[i][j]=matrix1.m_Storage.M[i][j]-matrix2.m_Storage.M[i][j];
	return ret;
}

//M*M
template <typename T,
		  template <typename,int,int> class StorageTrait>
Matrix<T,3,3,StorageTrait> operator * (const Matrix<T,3,3,StorageTrait> &matrix1, const Matrix<T,3,3,StorageTrait> &matrix2)
{
	return Matrix<T,3,3,StorageTrait>( matrix1.m_Storage.M[0][0]*matrix2.m_Storage.M[0][0]+matrix1.m_Storage.M[0][1]*matrix2.m_Storage.M[1][0]+matrix1.m_Storage.M[0][2]*matrix2.m_Storage.M[2][0],
									   matrix1.m_Storage.M[0][0]*matrix2.m_Storage.M[0][1]+matrix1.m_Storage.M[0][1]*matrix2.m_Storage.M[1][1]+matrix1.m_Storage.M[0][2]*matrix2.m_Storage.M[2][1],
									   matrix1.m_Storage.M[0][0]*matrix2.m_Storage.M[0][2]+matrix1.m_Storage.M[0][1]*matrix2.m_Storage.M[1][2]+matrix1.m_Storage.M[0][2]*matrix2.m_Storage.M[2][2],
										   
									   matrix1.m_Storage.M[1][0]*matrix2.m_Storage.M[0][0]+matrix1.m_Storage.M[1][1]*matrix2.m_Storage.M[1][0]+matrix1.m_Storage.M[1][2]*matrix2.m_Storage.M[2][0],
									   matrix1.m_Storage.M[1][0]*matrix2.m_Storage.M[0][1]+matrix1.m_Storage.M[1][1]*matrix2.m_Storage.M[1][1]+matrix1.m_Storage.M[1][2]*matrix2.m_Storage.M[2][1],
									   matrix1.m_Storage.M[1][0]*matrix2.m_Storage.M[0][2]+matrix1.m_Storage.M[1][1]*matrix2.m_Storage.M[1][2]+matrix1.m_Storage.M[1][2]*matrix2.m_Storage.M[2][2],
									   
									   matrix1.m_Storage.M[2][0]*matrix2.m_Storage.M[0][0]+matrix1.m_Storage.M[2][1]*matrix2.m_Storage.M[1][0]+matrix1.m_Storage.M[2][2]*matrix2.m_Storage.M[2][0],
									   matrix1.m_Storage.M[2][0]*matrix2.m_Storage.M[0][1]+matrix1.m_Storage.M[2][1]*matrix2.m_Storage.M[1][1]+matrix1.m_Storage.M[2][2]*matrix2.m_Storage.M[2][1],
									   matrix1.m_Storage.M[2][0]*matrix2.m_Storage.M[0][2]+matrix1.m_Storage.M[2][1]*matrix2.m_Storage.M[1][2]+matrix1.m_Storage.M[2][2]*matrix2.m_Storage.M[2][2]);
}

//M*M
template <typename T,
		  int Rows1,
		  int Columns1,
		  int Rows2,
		  int Columns2,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Rows1,Columns2,StorageTrait> operator * (const Matrix<T,Rows1,Columns1,StorageTrait> &matrix1, const Matrix<T,Rows2,Columns2,StorageTrait> &matrix2)
{
	assert(Rows2==Columns1);

	Matrix<T,Rows1,Columns2,StorageTrait> ret;
	
	for(int i(0);i<Rows1;++i) for (int j(0);j<Columns2;++j) 
	{
		ret.m_Storage.M[i][j]=T();

		for (int k(0);k<Columns1;++k) (ret.m_Storage.M[i][j])+=(matrix1.m_Storage.M[i][k])*(matrix2.m_Storage.M[k][j]);
	}

	return ret;
}

//M==M
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
bool operator== (const Matrix<T,Rows,Columns,StorageTrait> &matrix1, const Matrix<T,Rows,Columns,StorageTrait> &matrix2)
{
	bool flag=true;
	T f(0.001f);

	for(int i(0);i<Rows;++i) 
	{
		for (int j(0);j<Columns;++j)
		{
			if(matrix1.m_Storage.M[i][j]!=matrix2.m_Storage.M[i][j])
			{
				flag=false;

				if (fabs(matrix1.m_Storage.M[i][j]-matrix2.m_Storage.M[i][j])<=f)
				{
					flag=true;
					continue;
				}

				goto result;
			}
		}
	}
result:	
	return flag;
}

//M!=M
template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
bool operator!= (const Matrix<T,Rows,Columns,StorageTrait> &matrix1, const Matrix<T,Rows,Columns,StorageTrait> &matrix2)
{
	bool flag=true;
	T f(0.001f);

	for(int i(0);i<Rows;++i) 
	{
		for (int j(0);j<Columns;++j)
		{
			if(matrix1.m_Storage.M[i][j]==matrix2.m_Storage.M[i][j])
			{
				flag=false;
				goto result;
			}
			if (fabs(matrix1.m_Storage.M[i][j]-matrix2.m_Storage.M[i][j])<=f)
			{
				flag=false;
				goto result;
			}
		}
	}
result:	
	return flag;
}


template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
Matrix<T,Columns,Rows,StorageTrait> Transpose(const Matrix<T,Rows,Columns,StorageTrait>& matrix)
{
	Matrix<T,Columns,Rows,StorageTrait> ret;
	for(int i(0);i<Rows;++i) for (int j(0);j<Columns;++j) ret.m_Storage.M[j][i]=matrix.m_Storage.M[i][j];
	
	return ret;
}

