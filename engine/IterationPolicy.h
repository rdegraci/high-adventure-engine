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
#include <vector>

template <typename ChunkedTrait>
class Chunked
{
protected:
	Chunked(){}
};


//Chunked iteration of a CSM data stream
template <>
class Chunked<CSM_DataStream>
{

protected:
	Chunked(){}

public:

	CSM_DataStream::Count GetMeshCount(const CSM_DataStream::RawStreamData& rawstreamdata) const
	{
		return rawstreamdata.m_MeshHeader.meshes;
	}

	CSM_DataStream::Count GetTotalSurfaceCount(const CSM_DataStream::RawStreamData& rawstreamdata) const
	{
		//returns: Totalmeshes * surfacecount-per-mesh

		size_t meshcount=rawstreamdata.m_MeshHeader.meshes;
		CSM_DataStream::Count surfacecount=CSM_DataStream::Count();

		for( size_t m(0); m<meshcount; ++m) surfacecount+=rawstreamdata.m_pMeshData[m].surfaces;

		return surfacecount;
	}

	CSM_DataStream::Count GetTotalGroups(const CSM_DataStream::RawStreamData& rawstreamdata) const
	{
		return rawstreamdata.m_GroupHeader.groups;
	}

	CSM_DataStream::Count GetGroupID(const CSM_DataStream::RawStreamData& rawstreamdata, const CSM_DataStream::Index meshindex) const
	{
		return rawstreamdata.m_pMeshData[meshindex].visgroup;
	}

	CSM_DataStream::Count GetTotalIndexedVertexCount(const CSM_DataStream::RawStreamData& rawstreamdata, const CSM_DataStream::Index meshindex) const
	{
		//Return aggregate total _indexed_ vertex count for all surfaces in the mesh specified by meshindex
		CSM_DataStream::Count count(0);
		for (CSM_DataStream::Count s=0; s<(rawstreamdata.m_pMeshData[meshindex].surfaces); ++s)
		{
			count+=rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vertices;
		}
		return count;
	}

	CSM_DataStream::Count GetTotalTriIndexCount(const CSM_DataStream::RawStreamData& rawstreamdata, const CSM_DataStream::Index meshindex) const
	{
		//Return aggregate total index count for all surfaces in the mesh specified by meshindex
		CSM_DataStream::Count count(0);
		for (CSM_DataStream::Count s=0; s<(rawstreamdata.m_pMeshData[meshindex].surfaces); ++s)
		{
			count+=(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].triangles * 3);	//Three triangle points
		}
		return count;
	}

	CSM_DataStream::Count GetSurfaceCount(const CSM_DataStream::RawStreamData& rawstreamdata, CSM_DataStream::Index meshindex) const
	{
		//returns: surfacecount-per-mesh
		return (rawstreamdata.m_pMeshData[meshindex].surfaces);
	}

	int GetTexturePathsPerMesh(const CSM_DataStream::RawStreamData& rawstreamdata, CSM_DataStream::TexturePathsBuffer& buffer, const CSM_DataStream::Index meshindex) const
	{
		//Returns: current mesh index and all texture paths for all surfaces of the current mesh index

		for (CSM_DataStream::Count s=0; s<(rawstreamdata.m_pMeshData[meshindex].surfaces); ++s)
		{
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].texturename);
		}

		return (rawstreamdata.m_pMeshData[meshindex].surfaces);
	}

	int GetVertexDataPerSurface(const CSM_DataStream::RawStreamData& rawstreamdata, CSM_DataStream::VertexComponentBuffer& buffer, const CSM_DataStream::Index meshindex) const
	{
		//Returns: current mesh index and all vertices for all surfaces of the current mesh index
		//Note: the vertices are indexed

		static int s(0);

		for (int v=0; v<(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vertices); ++v)
		{
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vdata[v].x);
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vdata[v].y);
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vdata[v].z);
		}

		int surfaces=(rawstreamdata.m_pMeshData[meshindex].surfaces)-1;	//zero index
		int currentsurface=s;

		//Roll-over the surface, if we've iterated through the surfaces
		s=(s==(surfaces)) ? 0 : ++s;

		return currentsurface;
	}

	int GetNormalDataPerSurface(const CSM_DataStream::RawStreamData& rawstreamdata,CSM_DataStream::VertexComponentBuffer& buffer, const CSM_DataStream::Index meshindex) const
	{
		//Returns: current mesh index and all normals for all surfaces of the current mesh index
		//Note: the normals are per vertex

		static int s(0);

		for (int v=0; v<(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vertices); ++v)
		{
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vdata[v].nx);
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vdata[v].ny);
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vdata[v].nz);
		}

		int surfaces=(rawstreamdata.m_pMeshData[meshindex].surfaces)-1;	//zero index
		int currentsurface=s;

		//Roll-over the surface, if we've iterated through the surfaces
		s=(s==surfaces) ? 0 : ++s;

		return currentsurface;
	}

	int GetTextureCoordDataPerSurface(const CSM_DataStream::RawStreamData& rawstreamdata, CSM_DataStream::PositionBuffer& buffer, const CSM_DataStream::Index meshindex) const
	{
		//Returns: current mesh index and all texture coords for all surfaces of the current mesh index
		//Note: the texture coords are per vertex

		static int s(0);

		for (int v=0; v<(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vertices); ++v)
		{
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vdata[v].Utexture);
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].vdata[v].Vtexture);
		}

		int surfaces=(rawstreamdata.m_pMeshData[meshindex].surfaces)-1;	//zero index
		int currentsurface=s;

		//Roll-over the surface, if we've iterated through the surfaces
		s=(s==surfaces) ? 0 : ++s;

		return currentsurface;
	}

	int GetTriIndexPerSurface(const CSM_DataStream::RawStreamData& rawstreamdata, CSM_DataStream::IndexBuffer& buffer, const CSM_DataStream::Index meshindex) const
	{
		//Returns: current mesh index and all tri-indices for all surfaces of the current mesh index
		//Note: the tri-indices are 'per triangle'

		static int s(0);

		for (int t=0; t<(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].triangles); ++t)
		{
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].tdata[t].x);
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].tdata[t].y);
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].tdata[t].z);
		}

		int surfaces=(rawstreamdata.m_pMeshData[meshindex].surfaces)-1;	//zero index
		int currentsurface=s;

		//Roll-over the surface, if we've iterated through the surfaces
		s=(s==surfaces) ? 0 : ++s;

		return currentsurface;
	}

	int GetLineIndexPerSurface(const CSM_DataStream::RawStreamData& rawstreamdata, CSM_DataStream::IndexBuffer& buffer, const CSM_DataStream::Index meshindex) const
	{
		//Returns: current mesh index and all line-indices for all surfaces of the current mesh index
		//Note: the tri-indices are 'per triangle'
		static int s(0);

		for (int l=0; l<(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].lines); ++l)
		{
			//Tri Index for surface (s)
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].ldata[l].a);
			buffer.push_back(rawstreamdata.m_pMeshData[meshindex].surfacedata[s].ldata[l].b);
		}

		int surfaces=(rawstreamdata.m_pMeshData[meshindex].surfaces)-1;	//zero index
		int currentsurface=s;

		//Roll-over the surface, if we've iterated through the surfaces
		s=(s==surfaces) ? 0 : ++s;

		return currentsurface;
	}
};