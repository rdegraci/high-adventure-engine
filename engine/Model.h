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

#include "TextureManager.h"
#include "MeshRepr.h"
#include "DataStream.h"

//Interface
class Model
{
protected:

	long m_GroupID;

public:
	typedef DataStream<CSM_DataStream,StructuredStream, Chunked> CSM_Stream;
	typedef MeshBufferRepr<VertexBuffer,StructureOfArrays> MeshVertexBuffer;

	Model(){}
	virtual ~Model(){}

	virtual void Create(CSM_Stream& stream,TextureManager& texturemanager,size_t index)=0;
	virtual void Draw()=0;
	virtual MeshVertexBuffer GetMeshVertexBuffer()=0;

	long GetGroupID()
	{
		return m_GroupID;
	}
};



class DynamicModel : public Model
{
	typedef DataStream<CSM_DataStream,StructuredStream, Chunked> CSM_Stream;
	typedef MeshRepr<DynamicGeometry,VertexArray> DynamicMesh;

	DynamicMesh* m_pMesh;
	

public:

	DynamicModel() : m_pMesh(0)
	{
	}

	virtual ~DynamicModel()
	{
		delete m_pMesh;
		m_pMesh=0;
	}



	virtual void Create(CSM_Stream& stream,TextureManager& texturemanager,size_t index)
	{
		//TODO: use MD3_Stream instead of CSM_Stream
		assert(0);

		stream.SetMeshPosition(index);
		m_GroupID=stream.GetGroupIDAtMeshPosition();
		m_pMesh=new DynamicMesh(stream,texturemanager);
	}

	virtual void Draw()
	{
		m_pMesh->Draw();
	}
};


class StaticModel : public Model
{
	typedef DataStream<CSM_DataStream,StructuredStream, Chunked> CSM_Stream;
	typedef MeshRepr<StaticGeometry,DisplayList> StaticMesh;
	typedef MeshBufferRepr<VertexBuffer,StructureOfArrays> MeshVertexBuffer;


	StaticMesh* m_pMesh;
	long m_GroupID;

	MeshVertexBuffer m_MeshVertexBuffer;	//Used for physics

public:

	StaticModel() : m_pMesh(0)
	{
	}

	virtual ~StaticModel()
	{
		delete m_pMesh;
		m_pMesh=0;
	}

	long GetGroupID()
	{
		return m_GroupID;
	}

	virtual void Create(CSM_Stream& stream,TextureManager& texturemanager,size_t index)
	{
		stream.SetMeshPosition(index);
		m_GroupID=stream.GetGroupIDAtMeshPosition();
		m_pMesh=new StaticMesh(stream,texturemanager);

		m_MeshVertexBuffer.Create(stream);
	}

	virtual void Draw()
	{
		m_pMesh->Draw();
	}

	virtual MeshVertexBuffer GetMeshVertexBuffer()
	{
		return m_MeshVertexBuffer;
	}
};



