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

	Rodney Degracia
*/



#pragma once

#include "MemorySegment.h"
#include "MemoryFrame.h"


/*
	Simple Memory Manager

	Creates Logical Frames (arrays) 
	Re-uses reclaimed Logical Frames if possible.

	Checks for possible memory leaks, in DEBUG mode
*/

template <typename TFrame, int memsize=64000,typename TSegment=TFrame>
class MemoryManager
{
	MemorySegment<TSegment> m_Segment;


#ifdef _DEBUG
	std::vector<MemoryFrame<TFrame,TSegment>*> m_StackInUse;	//When debugging, track Frames in use
#endif _DEBUG

	std::vector<MemoryFrame<TFrame,TSegment>*> m_StackFree;
public:

	MemoryManager() 
	{
		m_Segment.Create(memsize);		
	}

	~MemoryManager()
	{
		m_Segment.Reclaim();

		std::vector<MemoryFrame<TFrame,TSegment>*>::iterator i;

		for(i=m_StackFree.begin(); i!=m_StackFree.end();++i)
		{
			delete (*i);
			*i=0;
		}
		m_StackFree.clear();

#ifdef _DEBUG
		//Check for memory manager leaks
		//If these assert then that means that some Frames were not reclaimed
		assert(m_StackInUse.empty());
#endif _DEBUG

	}

	MemoryFrame<TFrame,TSegment>* NewMemoryFrame(size_t size, size_t offset)
	{
		if(m_StackFree.empty())
		{
			MemoryFrame<TFrame,TSegment>* pFrame=new MemoryFrame<TFrame,TSegment>;
			pFrame->SetMemorySegment(&(m_Segment));
			pFrame->Create(size,offset);

#ifdef _DEBUG
			//Track Frame
			m_StackInUse.push_back(pFrame);
#endif _DEBUG

			return pFrame;
		}
		else
		{
			MemoryFrame<TFrame,TSegment>* pFrame=m_StackFree.back();
			m_StackFree.pop_back();
			pFrame->Create(size,offset);

#ifdef _DEBUG
			//Track Frame
			m_StackInUse.push_back(pFrame);
#endif _DEBUG

			return pFrame;
		}
	}

	void DeleteMemoryFrame(MemoryFrame<TFrame,TSegment>*& pFrame)
	{

#ifdef _DEBUG
		//Recover Frame from debugging stack
		bool found=false;
		std::vector<MemoryFrame<TFrame,TSegment>*>::iterator i;
		for(i=m_StackInUse.begin(); i!=m_StackInUse.end();++i)
		{	
			if(*i==pFrame)
			{
				found=true;
				m_StackInUse.erase(i);
				break;
			}
		}
		//If asserts, then attempting to push a non-memory-manager-created
		//Frame onto the Free Frame stack
		assert(found);
#endif _DEBUG

		m_StackFree.push_back(pFrame);

		//We zero pFrame, since it's 'reclaimed'
		pFrame=0;
	}
};

