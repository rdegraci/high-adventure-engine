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

#include "MemorySegment.h"

//class MemorySegment;

template <typename TFrame,
		  typename TSegment=TFrame>
class MemoryFrame
{
	size_t m_Size;
	size_t m_OffSet;
	
	MemorySegment<TSegment>* m_pMemorySegment;
	TFrame* m_pFrameStart;
public:

	MemoryFrame() : m_Size(),
					m_OffSet(),
					m_pFrameStart(0),
					m_pMemorySegment(0)
	{
	}

	void SetMemorySegment(MemorySegment<TSegment>* pMemorySegment)
	{
		assert(pMemorySegment);

		m_pMemorySegment=pMemorySegment;
	}

	void Create(size_t size, size_t offset,MemorySegment<TSegment>* pMemorySegment)
	{
		assert(pMemorySegment);

		m_Size=size;
		m_OffSet=offset;

		m_pFrameStart=pMemorySegment->GetStart()+(offset*pMemorySegment->GetTypeSize());
	}

	void Create(size_t size, size_t offset)
	{
		assert(m_pMemorySegment);

		m_Size=size;
		m_OffSet=offset;

		m_pFrameStart=m_pMemorySegment->GetStart()+(offset*m_pMemorySegment->GetSegmentTypeSize());
	}

	size_t GetElementCount()
	{
		return m_Size;
	}

	size_t GetOffSet()
	{
		return m_OffSet;
	}

	size_t GetDatumSize()
	{
		return sizeof(TFrame);
	}

	size_t GetFrameByteSize()
	{
		return m_Size*sizeof(TFrame);
	}

	TFrame* GetFrameDataStart()
	{
		assert (m_pFrameStart);

		return m_pFrameStart;
	}
};