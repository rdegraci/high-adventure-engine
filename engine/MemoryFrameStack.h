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

#include "MemoryManager.h"
#include "MemoryFrame.h"

template <typename T, 
		  int memorysize=640000>
class MemoryFrameStack
{
	size_t m_nStackPointer;
	MemoryManager<T,memorysize> m_MemoryManager;
	std::vector<MemoryFrame<T>*> m_Stack;

public:
	MemoryFrameStack() : m_nStackPointer(0)
	{}

	~MemoryFrameStack()
	{
		Reclaim();

		//The stack pointer should be at zero
		assert(m_nStackPointer==0);
	}

	void Reclaim()
	{
		size_t count=m_Stack.size();
		for(size_t i(0);i!=count;++i)
		{
			MemoryFrame<T>* pFrame=m_Stack.back();
			Pop(pFrame);
		}
	}

	MemoryFrame<T>* Push(size_t size)
	{
		//Are we out of memory!?
		assert((m_nStackPointer+size)<=memorysize);

		MemoryFrame<T>* pFrame=m_MemoryManager.NewMemoryFrame(size,m_nStackPointer);
		m_nStackPointer+=size;
		m_Stack.push_back(pFrame);
		return pFrame;
	}

	MemoryFrame<T>* Push(MemoryFrame<T>* pFrame)
	{
		size_t elements=pFrame->GetElementCount();

		//Are we out of memory!?
		assert((m_nStackPointer+elements)<=memorysize);

		MemoryFrame<T>* pNewFrame=m_MemoryManager.NewMemoryFrame(elements,m_nStackPointer);
		m_nStackPointer+=elements;

		T* pSourceData=pFrame->GetFrameDataStart();
		T* pTargetData=pNewFrame->GetFrameDataStart();

		assert(pSourceData);
		assert(pTargetData);
		for(size_t i(0);i<elements;++i)
		{
			*(pTargetData+i)=*(pSourceData+i);
		}

		m_Stack.push_back(pNewFrame);

		return pNewFrame;
	}

	void Pop(MemoryFrame<T>*& pPopFrame)
	{
		MemoryFrame<T>* pFrame=m_Stack.back();

		//Verify that the Frame to pop is actually
		//at the top of the Stack
		assert(pPopFrame==pFrame);

		m_Stack.pop_back();
		m_nStackPointer-=pFrame->GetElementCount();
		m_MemoryManager.DeleteMemoryFrame(pFrame);
		pFrame=0;

		//The MemoryManager reclaims the Frame
		//therefore we clear the pPopFrame
		pPopFrame=0;
	}
};