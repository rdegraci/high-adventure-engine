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
#include "SurfaceReprTrait.h"
#include "StoragePolicy.h"


template <typename SurfaceReprTrait,
		  template <typename> class SurfaceStoragePolicy>
class SurfaceRepr 
{
public:
};


template <>
class SurfaceRepr<Polygonal,DisplayList> : public DisplayList<Polygonal>
{
	Polygonal::TextureObjectID		m_TextureID;
	Polygonal::DisplayListID	m_DisplayListID;

public:
	SurfaceRepr(){}

	void SetTextureID(Polygonal::TextureObjectID id)
	{
		m_TextureID=id;
	}

	Polygonal::TextureObjectID GetTextureID()
	{
		return m_TextureID;
	}

	void SetDisplayListID(Polygonal::DisplayListID displaylistid)
	{
		m_DisplayListID=displaylistid;
	}

	Polygonal::DisplayListID GetDisplayListID()
	{
		return m_DisplayListID;
	}
};


template <>
class SurfaceRepr<Polygonal,VertexArray> : public VertexArray<Polygonal>
{
	GLuint	m_Start;
	GLuint	m_End;

	Polygonal::TextureObjectID		m_TextureID;
	Polygonal::VertexData*			m_pVertexData;
	Polygonal::Perf_Index*			m_pIndexData;
	Polygonal::Count				m_ElementCount;

public:
	SurfaceRepr(){}

	void SetIndexData(Polygonal::IndexData* pIndexData)
	{
		assert(m_pIndexData);
		m_pIndexData=pIndexData;
	}

	Polygonal::IndexData* GetIndexData()
	{
		return m_pIndexData;
	}

	void SetVertexData(Polygonal::VertexData* pVertexData)
	{
		assert(m_pVertexData);
		m_pVertexData=pVertexData;
	}

	Polygonal::VertexData* GetVertexData()
	{
		return m_pVertexData;
	}

	void SetTextureID(Polygonal::TextureObjectID id)
	{
		m_TextureID=id;
	}

	GLuint GetTextureID()
	{
		return m_TextureID;
	}

	void SetIndexRangeStart(GLuint start)
	{
		m_Start=start;
	}

	void SetIndexRangeEnd(GLuint end)
	{
		m_End=end;
	}

	GLuint GetIndexRangeEnd()
	{
		return m_End;
	}

	GLuint GetIndexRangeStart()
	{
		return m_Start;
	}

	void SetElementCount( Polygonal::Count count)
	{
		m_ElementCount=count;
	}

	Polygonal::Count GetElementCount()
	{
		return m_ElementCount;
	}
};


