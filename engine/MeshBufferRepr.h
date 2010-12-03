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

#include "MeshBufferTrait.h"
#include "StoragePolicy.h"

template <typename MeshBufferReprTrait=VertexBuffer,
		  template <typename> class StoragePolicy=StructureOfArrays>
class MeshBufferRepr : public StoragePolicy<MeshBufferReprTrait>
{
	bool m_Originating;

public:
	//Public for speed
	typename MeshBufferReprTrait::ArrayData m_ArrayData;

	MeshBufferRepr() : m_Originating(false)
	{}

	MeshBufferRepr(const MeshBufferRepr<MeshBufferReprTrait>& mbr)
	{
		//Copy the array data, but we do not own it
		m_Originating=false;
		m_ArrayData=mbr.m_ArrayData;
	}

	~MeshBufferRepr()
	{
		//Reclaim the array data if we own it
		if(m_Originating) StoragePolicy<MeshBufferReprTrait>::Reclaim();
	}

	MeshBufferRepr<MeshBufferReprTrait>& operator=(const MeshBufferRepr<MeshBufferReprTrait>& mbr)
	{
		if(&mbr==this) return *this;

		//Copy the array data, but we do not own it
		m_Originating=false;
		m_ArrayData=mbr.m_ArrayData;

		return *this;
	}

	typename MeshBufferReprTrait::ArrayData& GetArrayData()
	{
		return m_ArrayData;
	}

	void Create(typename MeshBufferReprTrait::DataStream& datastream)
	{
		m_ArrayData=StoragePolicy<MeshBufferReprTrait>::Create(datastream);
		m_Originating=true;
	}

	void Create(typename MeshBufferReprTrait::Count elements)
	{
		m_ArrayData=StoragePolicy<MeshBufferReprTrait>::Create(elements);
		m_Originating=true;
	}

	typename MeshBufferReprTrait::Count GetCount()
	{
		return m_ArrayData.length;
	}
};