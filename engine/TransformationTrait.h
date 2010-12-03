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

#include "Matrix.h"
#include "Vector.h"
#include "StorageTrait.h"

template<typename T>
class Matrix3x3;

template<typename T>
class Vector1x3;

template<typename T>
class Vector3x1;

template <typename T,
		  int Rows,
		  int Columns,
		  template <typename,int,int> class StorageTrait>
class Vector;


struct Real3
{
	typedef Matrix3x3<float> RotationMatrix;
	typedef Vector3x1<float> ColumnVector;
	typedef Vector1x3<float> RowVector;

	//We represent a four tuple with a 1x4 row vector
	typedef Vector<float,1,4,Storage2Dim> Tuple4;
	
	typedef float One;
};

