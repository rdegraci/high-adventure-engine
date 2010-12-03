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

#include "DataStream.h"
#include "Model.h"
#include "Matrix.h"
#include <vector>

class Model;

struct VertexBuffer
{
	typedef float VectorComponent;
	typedef long Count;
	typedef VectorComponent* VectorComponentBuffer;

	typedef float VertexComponent;
	typedef std::vector<VertexComponent> VertexComponentBuffer;

	typedef struct _RawComponentData
	{
		Count length;
		VectorComponentBuffer pXCoords;
		VectorComponentBuffer pYCoords;
		VectorComponentBuffer pZCoords;

		_RawComponentData()
		{
			length=Count();
			pXCoords=VectorComponentBuffer();
			pYCoords=VectorComponentBuffer();
			pZCoords=VectorComponentBuffer();
		}

		void Reclaim()
		{
			assert(pXCoords);
			delete [] pXCoords;
			pXCoords=0;

			assert(pYCoords);
			delete [] pYCoords;
			pYCoords=0;

			assert(pZCoords);
			delete [] pZCoords;
			pZCoords=0;
		}

		void Create(Count length)
		{
			pXCoords=new VectorComponent[length];
			pYCoords=new VectorComponent[length];
			pZCoords=new VectorComponent[length];
		}

		void Insert(VectorComponent x,VectorComponent y, VectorComponent z, Count index)
		{
			pXCoords[index]=x;
			pYCoords[index]=y;
			pZCoords[index]=z;
		}

	} ArrayData;

	typedef DataStream<CSM_DataStream,StructuredStream,Chunked> DataStream;
};

struct SortBuffer
{
	typedef long Count;
	typedef Model DrawableObject;

	typedef DrawableObject* DrawableObjectPointer;
	typedef Matrix3x3<float>* TranformationMatrixPointer;

	typedef DrawableObjectPointer* DrawableObjectBuffer;
	typedef TranformationMatrixPointer* TransformationMatrixBuffer;

	typedef struct _RawComponentData
	{
		Count length;
		DrawableObjectBuffer* pDrawableObjectBuffer;
		TransformationMatrixBuffer* pTranslations;
		TransformationMatrixBuffer* pRotations;
		TransformationMatrixBuffer* pScalings;

		void Reclaim()
		{
			delete [] pDrawableObjectBuffer;
			pDrawableObjectBuffer=0;

			delete [] pTranslations;
			pTranslations=0;

			delete [] pRotations;
			pRotations=0;

			delete [] pScalings;
			pScalings=0;
		}

		void Create(Count length)
		{
			pDrawableObjectBuffer=new DrawableObjectBuffer[length];
			pTranslations=new TransformationMatrixBuffer[length];
			pRotations=new TransformationMatrixBuffer[length];
			pScalings=new TransformationMatrixBuffer[length];
		}

		void Insert(DrawableObjectPointer* pDO,
				    TranformationMatrixPointer* pT, 
					TranformationMatrixPointer* pR,
					TranformationMatrixPointer* pS,
					Count index)
		{
			pDrawableObjectBuffer[index]=pDO;
			pTranslations[index]=pT;
			pRotations[index]=pR;
			pScalings[index]=pS;
		}
	} ArrayData;
};