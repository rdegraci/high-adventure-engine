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

#include "DataStreamTrait.h"
#include "SerializationPolicy.h"
#include "IterationPolicy.h"


/*
	DataStream represents a graphics Format

	SerializationPolicy determines how the stream is serialized

	IterationPolicy determines how to iterate the stream
*/

template <typename DataStreamTrait,
		  template <typename> class SerializationPolicy=StructuredStream,
		  template <typename> class IterationPolicy=Chunked_Data>
class DataStream : public SerializationPolicy<DataStreamTrait>,
				   public IterationPolicy<DataStreamTrait>
{
	typename DataStreamTrait::RawStreamData m_RawStreamData;	//Raw Stream data storage
	typename DataStreamTrait::FilePointer m_pFile;
	typename DataStreamTrait::Index m_MeshIndex;
public:

	DataStream() : m_pFile(0)
	{}

	bool Open(const char* pFileName)
	{
		if (m_pFile) fclose(m_pFile);

		typename DataStreamTrait::FilePointer fh = fopen(pFileName, "rb");
		if (!fh)
		{
			printf("Can't open DataStream file '%s'\n", pFileName);
			return 1;
		}
		m_pFile=fh;

		return 0;
	};

	void Read()
	{
		assert(m_pFile!=0);
		SerializationPolicy<DataStreamTrait>::LoadFileFormat(m_pFile);
		m_RawStreamData=SerializationPolicy<DataStreamTrait>::GetRawStreamData();
	}

	void SetMeshPosition(typename DataStreamTrait::Index meshindex)
	{
		m_MeshIndex=meshindex;
	}

	typename DataStreamTrait::Count GetMeshCount() const
	{
		return IterationPolicy<DataStreamTrait>::GetMeshCount(m_RawStreamData);
	}

	typename DataStreamTrait::Count GetTotalSurfaceCount() const
	{
		return IterationPolicy<DataStreamTrait>::GetTotalSurfaceCount(m_RawStreamData);
	}

	typename DataStreamTrait::Count GetTotalGroups() const
	{
		return IterationPolicy<DataStreamTrait>::GetTotalGroups(m_RawStreamData);
	}

	typename DataStreamTrait::Count GetGroupIDAtMeshPosition() const
	{
		return IterationPolicy<DataStreamTrait>::GetGroupID(m_RawStreamData,m_MeshIndex);
	}

	typename DataStreamTrait::Count GetTotalIndexedVertexCountAtMeshPosition() const
	{
		//Returns total indexed vertex count
		return IterationPolicy<DataStreamTrait>::GetTotalIndexedVertexCount(m_RawStreamData,m_MeshIndex);
	}

	typename DataStreamTrait::Count GetTotalTriIndexCountAtMeshPosition() const
	{
		//Returns total indexed vertex count
		return IterationPolicy<DataStreamTrait>::GetTotalTriIndexCount(m_RawStreamData,m_MeshIndex);
	}

	typename DataStreamTrait::Count GetSurfaceCountAtMeshPosition() const
	{
		return IterationPolicy<DataStreamTrait>::GetSurfaceCount(m_RawStreamData,m_MeshIndex);
	}

	int GetTexturePathPerSurface(CSM_DataStream::TexturePathsBuffer& texturepaths) const
	{
		int surface=IterationPolicy<CSM_DataStream>::GetTexturePathsPerMesh(m_RawStreamData,texturepaths,m_MeshIndex);
		return surface;
	}

	int GetVertexDataPerSurface(CSM_DataStream::VertexComponentBuffer& vertices) const
	{
		int surface=IterationPolicy<CSM_DataStream>::GetVertexDataPerSurface(m_RawStreamData,vertices,m_MeshIndex);
		return surface;
	}

	int GetNormalDataPerSurface(CSM_DataStream::VertexComponentBuffer& normals) const
	{
		int surface=IterationPolicy<CSM_DataStream>::GetNormalDataPerSurface(m_RawStreamData,normals,m_MeshIndex);
		return surface;
	}

	int GetTextureCoordDataPerSurface(CSM_DataStream::PositionBuffer& texturecoords) const
	{
		int surface=IterationPolicy<CSM_DataStream>::GetTextureCoordDataPerSurface(m_RawStreamData,texturecoords,m_MeshIndex);
		return surface;
	}

	int GetVertexIndexPerSurface(CSM_DataStream::IndexBuffer& index) const
	{
		int surface=IterationPolicy<CSM_DataStream>::GetIndicesPerSurface(m_RawStreamData,index,m_MeshIndex);
		return surface;
	}

	int GetTriIndexPerSurface(CSM_DataStream::IndexBuffer& index) const
	{
		int surface=IterationPolicy<CSM_DataStream>::GetTriIndexPerSurface(m_RawStreamData,index,m_MeshIndex);
		return surface;
	}
};


