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

#include <string>
#include <cassert>
#include <iostream>

#include "DataStreamTrait.h"


template <typename StructuredStreamTrait>
class StructuredStream
{
};


template <>
class StructuredStream<CSM_DataStream>
{

public:

	CSM_DataStream::RawStreamData m_RawStreamData;

	//Toggle used for cleaning up data structures
	bool m_bIsClean;

	CSM_DataStream::Header m_Header;
	CSM_DataStream::Group_Header m_GroupHeader;
	CSM_DataStream::Vis_Group_Header m_VisGroupHeader;
	CSM_DataStream::LightMap_Header m_LightMapHeader;
	CSM_DataStream::Mesh_Header m_MeshHeader;
	CSM_DataStream::Entities_Header m_EntitiesHeader;

	//Data Buffers
	CSM_DataStream::Group_Data* m_pGroupData;
	CSM_DataStream::Vis_Group_Data* m_pVisGroupData;
	CSM_DataStream::LightMap_Data* m_pLightMapData;
	CSM_DataStream::Mesh_Data* m_pMeshData;
	CSM_DataStream::Surface_Data* m_pSurfaceData;
	CSM_DataStream::Entities_Data* m_pEntitiesData;
	CSM_DataStream::Camera_Data m_CameraData;

	CSM_DataStream::RawStreamData GetRawStreamData()
	{
		m_RawStreamData.m_Header=m_Header;
		m_RawStreamData.m_GroupHeader=m_GroupHeader;
		m_RawStreamData.m_VisGroupHeader=m_VisGroupHeader;
		m_RawStreamData.m_LightMapHeader=m_LightMapHeader;
		m_RawStreamData.m_MeshHeader=m_MeshHeader;
		m_RawStreamData.m_EntitiesHeader=m_EntitiesHeader;

		//Data Buffers
		m_RawStreamData.m_pGroupData=m_pGroupData;
		m_RawStreamData.m_pVisGroupData=m_pVisGroupData;
		m_RawStreamData.m_pLightMapData=m_pLightMapData;
		m_RawStreamData.m_pMeshData=m_pMeshData;
		m_RawStreamData.m_pSurfaceData=m_pSurfaceData;
		m_RawStreamData.m_pEntitiesData=m_pEntitiesData;

		return m_RawStreamData;
	}

protected:

	StructuredStream() :
	  	m_pGroupData(0),
		m_pVisGroupData(0),
		m_pLightMapData(0),
		m_pMeshData(0),
		m_pSurfaceData(0),
		m_pEntitiesData(0),
		m_bIsClean(true)
	{
	}

	void ReadGroups(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);

		m_pGroupData=new CSM_DataStream::Group_Data[m_GroupHeader.groups];
		assert(m_pGroupData);

		for(int i=0; i<m_GroupHeader.groups;++i)
		{
			long position = ftell(pFile);
			//std::cout<<"ByteOffset="<<position<<std::endl;

			fread(&(m_pGroupData[i].flags),sizeof(CSM_DataStream::FlagBuffer),1,pFile);
			fread(&(m_pGroupData[i].id),sizeof(CSM_DataStream::ID),1,pFile);

			CSM_DataStream::StringBuffer* pBuffer=new CSM_DataStream::StringBuffer;
			assert(pBuffer);
			CSM_DataStream::ReadBuffer(pBuffer,pFile);
			m_pGroupData[i].properties=pBuffer;
			
			fread(&(m_pGroupData[i].red),sizeof(CSM_DataStream::Color),1,pFile);
			fread(&(m_pGroupData[i].green),sizeof(CSM_DataStream::Color),1,pFile);
			fread(&(m_pGroupData[i].blue),sizeof(CSM_DataStream::Color),1,pFile);

			//std::cout<<"\tGroup#="<<i<<std::endl;
			//std::cout<<"\tProperties#="<<*(m_pGroupData[i].properties)<<std::endl;
			//std::cout<<"\tRGB="<<m_pGroupData[i].red<<
			//	","<<m_pGroupData[i].green<<","<<
			//	m_pGroupData[i].blue<<std::endl;
		}
	}

	void WriteGroups(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);
		assert(m_pGroupData);

		for(int i=0; i<m_GroupHeader.groups;++i)
		{
			fwrite(&(m_pGroupData[i].flags),sizeof(CSM_DataStream::FlagBuffer),1,pFile);
			fwrite(&(m_pGroupData[i].id),sizeof(CSM_DataStream::ID),1,pFile);

			CSM_DataStream::WriteBuffer(m_pGroupData[i].properties,pFile);

			fwrite(&(m_pGroupData[i].red),sizeof(CSM_DataStream::Color),1,pFile);
			fwrite(&(m_pGroupData[i].green),sizeof(CSM_DataStream::Color),1,pFile);
			fwrite(&(m_pGroupData[i].blue),sizeof(CSM_DataStream::Color),1,pFile);
		}
	}

	void ReadVisGroups(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);

		m_pVisGroupData=new CSM_DataStream::Vis_Group_Data[m_VisGroupHeader.groups];
		assert(m_pVisGroupData);

		for(int i=0; i<m_VisGroupHeader.groups;++i)
		{	
			long position = ftell(pFile);
			//std::cout<<"ByteOffset="<<position<<std::endl;

			fread(&(m_pVisGroupData[i].hiddenshown),sizeof(CSM_DataStream::Boolean),1,pFile);
			fread(&(m_pVisGroupData[i].red),sizeof(CSM_DataStream::Color),1,pFile);
			fread(&(m_pVisGroupData[i].green),sizeof(CSM_DataStream::Color),1,pFile);
			fread(&(m_pVisGroupData[i].blue),sizeof(CSM_DataStream::Color),1,pFile);

			//std::cout<<"\tVisGroup#="<<i<<std::endl;
			//std::cout<<"\thiddenshown="<<m_pVisGroupData[i].hiddenshown<<std::endl;
			//std::cout<<"\tRGB="<<m_pVisGroupData[i].red<<","<<
			//	m_pVisGroupData[i].green<<","<<
			//	m_pVisGroupData[i].blue<<std::endl;
		}
	}

	void WriteVisGroups(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);
		assert(m_pVisGroupData);

		for(int i=0; i<m_VisGroupHeader.groups;++i)
		{
			fwrite(&(m_pVisGroupData[i].hiddenshown),sizeof(CSM_DataStream::Boolean),1,pFile);
			fwrite(&(m_pVisGroupData[i].red),sizeof(CSM_DataStream::Color),1,pFile);
			fwrite(&(m_pVisGroupData[i].green),sizeof(CSM_DataStream::Color),1,pFile);
			fwrite(&(m_pVisGroupData[i].blue),sizeof(CSM_DataStream::Color),1,pFile);
		}
	}

	void ReadLightMaps(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);

		m_pLightMapData=new CSM_DataStream::LightMap_Data[m_LightMapHeader.lightmaps];
		assert(m_pLightMapData);

		for(int i=0; i<m_LightMapHeader.lightmaps;++i)
		{
			long position = ftell(pFile);
			//std::cout<<"ByteOffset="<<position<<std::endl;

			fread(&(m_pLightMapData[i].width),sizeof(CSM_DataStream::Scalar),1,pFile);
			fread(&(m_pLightMapData[i].height),sizeof(CSM_DataStream::Scalar),1,pFile);

			assert(m_pLightMapData[i].width>0);
			assert(m_pLightMapData[i].height>0);

			int buffersize=m_pLightMapData[i].width*m_pLightMapData[i].height;
			CSM_DataStream::Color* pBuffer=new CSM_DataStream::Color[buffersize];
			assert(pBuffer);
			m_pLightMapData[i].pRGBA_Data=pBuffer;
			fread(pBuffer,sizeof(CSM_DataStream::Color),buffersize,pFile);

			//std::cout<<"\tLighMap#="<<i<<std::endl;
			//std::cout<<"\tWidth="<<m_pLightMapData[i].width<<std::endl;
			//std::cout<<"\tHeight="<<m_pLightMapData[i].height<<std::endl;
		}
	}

	void WriteLightMaps(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);
		assert(m_pLightMapData);

		for(int i=0; i<m_LightMapHeader.lightmaps;++i)
		{
			fwrite(&(m_pLightMapData[i].width),sizeof(CSM_DataStream::Scalar),1,pFile);
			fwrite(&(m_pLightMapData[i].height),sizeof(CSM_DataStream::Scalar),1,pFile);

			assert(m_pLightMapData[i].pRGBA_Data);
			CSM_DataStream::Color* pBuffer=m_pLightMapData[i].pRGBA_Data;
			int buffersize=m_pLightMapData[i].width*m_pLightMapData[i].height;
			fwrite(pBuffer,sizeof(CSM_DataStream::Color),buffersize,pFile);
		}
	}

	void ReadMeshes(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);

		m_pMeshData=new CSM_DataStream::Mesh_Data[m_MeshHeader.meshes];
		assert(m_pMeshData);

		for(int i=0; i<m_MeshHeader.meshes; ++i)
		{
			long position = ftell(pFile);
			//std::cout<<"ByteOffset="<<position<<std::endl;

			fread(&(m_pMeshData[i].flags),sizeof(CSM_DataStream::FlagBuffer),1,pFile);
			fread(&(m_pMeshData[i].group),sizeof(CSM_DataStream::ID),1,pFile);

			CSM_DataStream::StringBuffer* pBuffer=new CSM_DataStream::StringBuffer;
			assert(pBuffer);
			CSM_DataStream::ReadBuffer(pBuffer,pFile);
			m_pMeshData[i].properties=pBuffer;

			fread(&(m_pMeshData[i].red),sizeof(CSM_DataStream::Color),1,pFile);
			fread(&(m_pMeshData[i].green),sizeof(CSM_DataStream::Color),1,pFile);
			fread(&(m_pMeshData[i].blue),sizeof(CSM_DataStream::Color),1,pFile);

			m_pMeshData[i].x=CSM_DataStream::ReadPosition(pFile);
			m_pMeshData[i].y=CSM_DataStream::ReadPosition(pFile);
			m_pMeshData[i].z=CSM_DataStream::ReadPosition(pFile);

			fread(&(m_pMeshData[i].visgroup),sizeof(CSM_DataStream::ID),1,pFile);
			fread(&(m_pMeshData[i].surfaces),sizeof(CSM_DataStream::Count),1,pFile);

			std::cout<<"\tMesh#="<<i<<std::endl;
			//std::cout<<"\tFlags="<<m_pMeshData[i].flags<<std::endl;
			//std::cout<<"\tGroup="<<m_pMeshData[i].group<<std::endl;
			//std::cout<<"\tProperties="<<*(m_pMeshData[i].properties)<<std::endl;
			//std::cout<<"\tRGB="<<m_pMeshData[i].red<<","<<
			//	m_pMeshData[i].green<<","<<
			//	m_pMeshData[i].blue<<std::endl;
			//std::cout<<"\txyz="<<m_pMeshData[i].x<<","<<
			//	m_pMeshData[i].y<<","<<
			//	m_pMeshData[i].z<<std::endl;
			//std::cout<<"\tVisGroup#="<<m_pMeshData[i].visgroup<<std::endl;
			std::cout<<"\tSurfaceCount="<<m_pMeshData[i].surfaces<<std::endl;

			CSM_DataStream::Count surfacecount=m_pMeshData[i].surfaces;
			if(surfacecount>0)
			{
				CSM_DataStream::Surface_Data* pSurfaceData=new CSM_DataStream::Surface_Data[surfacecount];
				assert(pSurfaceData);
				ReadSurfaceData(pSurfaceData,surfacecount,pFile);
				m_pMeshData[i].surfacedata=pSurfaceData;
			}
			else m_pMeshData[i].surfacedata=0;
		}
	}	

	void WriteMeshes(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);
		assert(m_pMeshData);

		for(int i=0; i<m_MeshHeader.meshes; ++i)
		{
			fwrite(&(m_pMeshData[i].flags),sizeof(CSM_DataStream::FlagBuffer),1,pFile);
			fwrite(&(m_pMeshData[i].group),sizeof(CSM_DataStream::ID),1,pFile);

			assert(m_pMeshData[i].properties);
			CSM_DataStream::WriteBuffer(m_pMeshData[i].properties,pFile);

			fwrite(&(m_pMeshData[i].red),sizeof(CSM_DataStream::Color),1,pFile);
			fwrite(&(m_pMeshData[i].green),sizeof(CSM_DataStream::Color),1,pFile);
			fwrite(&(m_pMeshData[i].blue),sizeof(CSM_DataStream::Color),1,pFile);

			CSM_DataStream::WritePosition(m_pMeshData[i].x,pFile);
			CSM_DataStream::WritePosition(m_pMeshData[i].y,pFile);
			CSM_DataStream::WritePosition(m_pMeshData[i].z,pFile);

			fwrite(&(m_pMeshData[i].visgroup),sizeof(CSM_DataStream::ID),1,pFile);
			fwrite(&(m_pMeshData[i].surfaces),sizeof(CSM_DataStream::Count),1,pFile);

			CSM_DataStream::Count surfacecount=m_pMeshData[i].surfaces;
			if(surfacecount>0)
			{
				WriteSurfaceData(m_pMeshData[i].surfacedata,surfacecount,pFile);
			}
		}
	}

	void ReadSurfaceData(CSM_DataStream::Surface_Data* pSurfaceBuffer,CSM_DataStream::Count surfaces, CSM_DataStream::FilePointer pFile)
	{
		assert(pSurfaceBuffer);
		assert(surfaces>0);
		assert(pFile);

		for(int i=0; i<surfaces; ++i)
		{
			long position = ftell(pFile);
			//std::cout<<"SurfaceByteOffset="<<position<<std::endl;

			fread(&(pSurfaceBuffer[i].flags),sizeof(CSM_DataStream::FlagBuffer),1,pFile);

			CSM_DataStream::StringBuffer* pBuffer=new CSM_DataStream::StringBuffer;
			assert(pBuffer);
			CSM_DataStream::ReadBuffer(pBuffer,pFile);
			pSurfaceBuffer[i].texturename=pBuffer;

			fread(&(pSurfaceBuffer[i].lightmap),sizeof(CSM_DataStream::ID),1,pFile);
			pSurfaceBuffer[i].uoffset=CSM_DataStream::ReadPosition(pFile);
			pSurfaceBuffer[i].voffset=CSM_DataStream::ReadPosition(pFile);
			pSurfaceBuffer[i].uscale=CSM_DataStream::ReadPosition(pFile);
			pSurfaceBuffer[i].vscale=CSM_DataStream::ReadPosition(pFile);
			pSurfaceBuffer[i].rotation=CSM_DataStream::ReadPosition(pFile);

			fread(&(pSurfaceBuffer[i].vertices),sizeof(CSM_DataStream::Count),1,pFile);
			fread(&(pSurfaceBuffer[i].triangles),sizeof(CSM_DataStream::Count),1,pFile);
			fread(&(pSurfaceBuffer[i].lines),sizeof(CSM_DataStream::Count),1,pFile);

			std::cout<<"\tSurface#="<<i<<std::endl;
			//std::cout<<"\tProperties="<<*(pSurfaceBuffer[i].texturename)<<std::endl;
			//std::cout<<"\tuLightMap="<<pSurfaceBuffer[i].lightmap<<std::endl;
			//std::cout<<"\tuOffset="<<pSurfaceBuffer[i].uoffset<<std::endl;
			//std::cout<<"\tvOffset="<<pSurfaceBuffer[i].voffset<<std::endl;
			//std::cout<<"\tuScale="<<pSurfaceBuffer[i].uscale<<std::endl;
			//std::cout<<"\tvScale="<<pSurfaceBuffer[i].vscale<<std::endl;
			//std::cout<<"\tRotation="<<pSurfaceBuffer[i].rotation<<std::endl;
			std::cout<<"\tVertexCount="<<pSurfaceBuffer[i].vertices<<std::endl;
			std::cout<<"\tTriangleCount="<<pSurfaceBuffer[i].triangles<<std::endl;
			std::cout<<"\tLineCount="<<pSurfaceBuffer[i].lines<<std::endl;

			CSM_DataStream::Count vertexcount=pSurfaceBuffer[i].vertices;
			if(vertexcount>0)
			{
				//Create dynamic buffer for the vertices
				CSM_DataStream::Vertex_Data* pVertexData=new CSM_DataStream::Vertex_Data[vertexcount];
				assert(pVertexData);
				ReadVertexData(pVertexData,vertexcount,pFile);
				pSurfaceBuffer[i].vdata=pVertexData;
				pVertexData=0;
			}
			else pSurfaceBuffer[i].vdata=0;

			CSM_DataStream::Count trianglecount=pSurfaceBuffer[i].triangles;
			if(trianglecount>0)
			{
				//Create dynamic buffer for the triangle polygons
				CSM_DataStream::Triangle_Data* pTriangleData=new CSM_DataStream::Triangle_Data[trianglecount];
				assert(pTriangleData);
				ReadTriangleData(pTriangleData,trianglecount,pFile);
				pSurfaceBuffer[i].tdata=pTriangleData;
				pTriangleData=0;
			}
			else pSurfaceBuffer[i].tdata=0;

			CSM_DataStream::Count linecount=pSurfaceBuffer[i].lines;
			if(linecount>0)
			{
				//Create dynamic buffer for the line data
				CSM_DataStream::Line_Data* pLineData=new CSM_DataStream::Line_Data[linecount];
				assert(pLineData);
				ReadLineData(pLineData,linecount,pFile);
				pSurfaceBuffer[i].ldata=pLineData;
				pLineData=0;
			}
			else pSurfaceBuffer[i].ldata=0;
		}
	}

	void WriteSurfaceData(CSM_DataStream::Surface_Data* pSurfaceBuffer,CSM_DataStream::Count surfaces, CSM_DataStream::FilePointer pFile)
	{
		assert(pSurfaceBuffer);
		assert(surfaces>0);
		assert(pFile);

		for(int i=0; i<surfaces; ++i)
		{
			fwrite(&(pSurfaceBuffer[i].flags),sizeof(CSM_DataStream::FlagBuffer),1,pFile);

			CSM_DataStream::WriteBuffer(pSurfaceBuffer[i].texturename,pFile);

			fwrite(&(pSurfaceBuffer[i].lightmap),sizeof(CSM_DataStream::ID),1,pFile);

			CSM_DataStream::WritePosition(pSurfaceBuffer[i].uoffset,pFile);
			CSM_DataStream::WritePosition(pSurfaceBuffer[i].voffset,pFile);
			CSM_DataStream::WritePosition(pSurfaceBuffer[i].uscale,pFile);
			CSM_DataStream::WritePosition(pSurfaceBuffer[i].vscale,pFile);
			CSM_DataStream::WritePosition(pSurfaceBuffer[i].rotation,pFile);

			fwrite(&(pSurfaceBuffer[i].vertices),sizeof(CSM_DataStream::Count),1,pFile);
			fwrite(&(pSurfaceBuffer[i].triangles),sizeof(CSM_DataStream::Count),1,pFile);
			fwrite(&(pSurfaceBuffer[i].lines),sizeof(CSM_DataStream::Count),1,pFile);

			CSM_DataStream::Count vertexcount=pSurfaceBuffer[i].vertices;
			if(vertexcount>0)
			{
				WriteVertexData(pSurfaceBuffer[i].vdata,vertexcount,pFile);
			}

			CSM_DataStream::Count trianglecount=pSurfaceBuffer[i].triangles;
			if(trianglecount>0)
			{
				WriteTriangleData(pSurfaceBuffer[i].tdata,trianglecount,pFile);
			}

			CSM_DataStream::Count linecount=pSurfaceBuffer[i].lines;
			if(linecount>0)
			{
				WriteLineData(pSurfaceBuffer[i].ldata,linecount,pFile);
			}
		}
	}

	void ReadVertexData(CSM_DataStream::Vertex_Data* pVertexBuffer,CSM_DataStream::Count vertexcount, CSM_DataStream::FilePointer pFile)
	{
		assert(pVertexBuffer);
		assert(vertexcount>0);
		assert(pFile);

		for(int i=0; i<vertexcount; ++i)
		{
			long position = ftell(pFile);
			//std::cout<<"VertexDataByteOffset="<<position<<std::endl;

			pVertexBuffer[i].x=CSM_DataStream::ReadPosition(pFile);
			pVertexBuffer[i].y=CSM_DataStream::ReadPosition(pFile);
			pVertexBuffer[i].z=CSM_DataStream::ReadPosition(pFile);

			pVertexBuffer[i].nx=CSM_DataStream::ReadPosition(pFile);
			pVertexBuffer[i].ny=CSM_DataStream::ReadPosition(pFile);
			pVertexBuffer[i].nz=CSM_DataStream::ReadPosition(pFile);

			fread(&(pVertexBuffer[i].red),sizeof(CSM_DataStream::Color),1,pFile);
			fread(&(pVertexBuffer[i].green),sizeof(CSM_DataStream::Color),1,pFile);
			fread(&(pVertexBuffer[i].blue),sizeof(CSM_DataStream::Color),1,pFile);

			pVertexBuffer[i].Utexture=CSM_DataStream::ReadPosition(pFile);
			pVertexBuffer[i].Vtexture=CSM_DataStream::ReadPosition(pFile);
			pVertexBuffer[i].Wtexture=CSM_DataStream::ReadPosition(pFile);

			pVertexBuffer[i].Ulightmap=CSM_DataStream::ReadPosition(pFile);
			pVertexBuffer[i].Vlightmap=CSM_DataStream::ReadPosition(pFile);
			pVertexBuffer[i].Wlightmap=CSM_DataStream::ReadPosition(pFile);

			std::cout<<"\tVertex#="<<i<<std::endl;
			std::cout<<"\tx,y,z="<<pVertexBuffer[i].x<<","<<
				pVertexBuffer[i].y<<","<<
				pVertexBuffer[i].z<<std::endl;
			std::cout<<"\tnx,ny,nz="<<pVertexBuffer[i].nx<<","<<
				pVertexBuffer[i].ny<<","<<
				pVertexBuffer[i].nz<<std::endl;
			//std::cout<<"\tR,G,B="<<pVertexBuffer[i].red<<","<<
			//	pVertexBuffer[i].green<<","<<
			//	pVertexBuffer[i].blue<<std::endl;
			std::cout<<"\tu,v,wTexture="<<pVertexBuffer[i].Utexture<<","<<
				pVertexBuffer[i].Vtexture<<","<<
				pVertexBuffer[i].Wtexture<<std::endl;
			//std::cout<<"\tu,v,wLightMap="<<pVertexBuffer[i].Ulightmap<<","<<
			//	pVertexBuffer[i].Vlightmap<<","<<
			//	pVertexBuffer[i].Wlightmap<<std::endl;
		}
	}

	void WriteVertexData(CSM_DataStream::Vertex_Data* pVertexBuffer,CSM_DataStream::Count vertexcount, CSM_DataStream::FilePointer pFile)
	{
		assert(pVertexBuffer);
		assert(vertexcount>0);
		assert(pFile);

		for(int i=0; i<vertexcount; ++i)
		{
			CSM_DataStream::WritePosition(pVertexBuffer[i].x,pFile);
			CSM_DataStream::WritePosition(pVertexBuffer[i].y,pFile);
			CSM_DataStream::WritePosition(pVertexBuffer[i].z,pFile);
			CSM_DataStream::WritePosition(pVertexBuffer[i].nx,pFile);
			CSM_DataStream::WritePosition(pVertexBuffer[i].ny,pFile);
			CSM_DataStream::WritePosition(pVertexBuffer[i].nz,pFile);

			fwrite(&(pVertexBuffer[i].red),sizeof(CSM_DataStream::Color),1,pFile);
			fwrite(&(pVertexBuffer[i].green),sizeof(CSM_DataStream::Color),1,pFile);
			fwrite(&(pVertexBuffer[i].blue),sizeof(CSM_DataStream::Color),1,pFile);

			CSM_DataStream::WritePosition(pVertexBuffer[i].Utexture,pFile);
			CSM_DataStream::WritePosition(pVertexBuffer[i].Vtexture,pFile);
			CSM_DataStream::WritePosition(pVertexBuffer[i].Wtexture,pFile);

			CSM_DataStream::WritePosition(pVertexBuffer[i].Ulightmap,pFile);
			CSM_DataStream::WritePosition(pVertexBuffer[i].Vlightmap,pFile);
			CSM_DataStream::WritePosition(pVertexBuffer[i].Wlightmap,pFile);
		}
	}

	void ReadTriangleData(CSM_DataStream::Triangle_Data* pTriangleBuffer,CSM_DataStream::Count trianglecount, CSM_DataStream::FilePointer pFile)
	{
		assert(pTriangleBuffer);
		assert(trianglecount>0);
		assert(pFile);

		for(int i=0; i<trianglecount; ++i)
		{
			long position = ftell(pFile);
			//std::cout<<"TriangleDataByteOffset="<<position<<std::endl;

			fread(&(pTriangleBuffer[i].x),sizeof(CSM_DataStream::Index),1,pFile);
			fread(&(pTriangleBuffer[i].y),sizeof(CSM_DataStream::Index),1,pFile);
			fread(&(pTriangleBuffer[i].z),sizeof(CSM_DataStream::Index),1,pFile);

			std::cout<<"\tTri-Index x,y,z="<<pTriangleBuffer[i].x<<","<<
				pTriangleBuffer[i].y<<","<<
				pTriangleBuffer[i].z<<std::endl;
		}
	}

	void WriteTriangleData(CSM_DataStream::Triangle_Data* pTriangleBuffer,CSM_DataStream::Count trianglecount, CSM_DataStream::FilePointer pFile)
	{
		assert(pTriangleBuffer);
		assert(trianglecount>0);
		assert(pFile);

		for(int i=0; i<trianglecount; ++i)
		{
			fwrite(&(pTriangleBuffer[i].x),sizeof(CSM_DataStream::Index),1,pFile);
			fwrite(&(pTriangleBuffer[i].y),sizeof(CSM_DataStream::Index),1,pFile);
			fwrite(&(pTriangleBuffer[i].z),sizeof(CSM_DataStream::Index),1,pFile);
		}
	}

	void ReadLineData(CSM_DataStream::Line_Data* pLineBuffer,CSM_DataStream::Count linecount, CSM_DataStream::FilePointer pFile)
	{
		assert(pLineBuffer);
		assert(linecount>0);
		assert(pFile);

		for(int i=0; i<linecount; ++i)
		{
			long position = ftell(pFile);
			//std::cout<<"LineDataByteOffset="<<position<<std::endl;

			fread(&(pLineBuffer[i].a),sizeof(CSM_DataStream::Index),1,pFile);
			fread(&(pLineBuffer[i].b),sizeof(CSM_DataStream::Index),1,pFile);

			//std::cout<<"\ta,b="<<pLineBuffer[i].a<<","<<
			//	pLineBuffer[i].b<<std::endl;
		}
	}

	void WriteLineData(CSM_DataStream::Line_Data* pLineBuffer,CSM_DataStream::Count linecount, CSM_DataStream::FilePointer pFile)
	{
		assert(pLineBuffer);
		assert(linecount>0);
		assert(pFile);

		for(int i=0; i<linecount; ++i)
		{
			fwrite(&(pLineBuffer[i].a),sizeof(CSM_DataStream::Index),1,pFile);
			fwrite(&(pLineBuffer[i].b),sizeof(CSM_DataStream::Index),1,pFile);
		}
	}

	void ReadEntities(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);

		m_pEntitiesData=new CSM_DataStream::Entities_Data[m_EntitiesHeader.entities];
		assert(m_pEntitiesData);

		for(int i=0; i<m_EntitiesHeader.entities; ++i)
		{
			long position = ftell(pFile);
			//std::cout<<"EntityByteOffset="<<position<<std::endl;

			fread(&(m_pEntitiesData[i].flags),sizeof(CSM_DataStream::FlagBuffer),1,pFile);
			fread(&(m_pEntitiesData[i].group),sizeof(CSM_DataStream::ID),1,pFile);

			CSM_DataStream::StringBuffer* pBuffer=new CSM_DataStream::StringBuffer;
			assert(pBuffer);
			CSM_DataStream::ReadBuffer(pBuffer,pFile);
			m_pEntitiesData[i].properties=pBuffer;

			m_pEntitiesData[i].x=CSM_DataStream::ReadPosition(pFile);
			m_pEntitiesData[i].y=CSM_DataStream::ReadPosition(pFile);
			m_pEntitiesData[i].z=CSM_DataStream::ReadPosition(pFile);

			//std::cout<<"\tEntity#="<<i<<std::endl;
			//std::cout<<"\tFlags="<<m_pEntitiesData[i].flags<<std::endl;
			//std::cout<<"\tGroup="<<m_pEntitiesData[i].group<<std::endl;
			//std::cout<<"\tProperties="<<*(m_pEntitiesData[i].properties)<<std::endl;
			//std::cout<<"\tx,y,z="<<m_pEntitiesData[i].x<<","<<
			//	m_pEntitiesData[i].y<<","<<
			//	m_pEntitiesData[i].z<<std::endl;
		}
	}

	void WriteEntities(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);
		assert(m_pEntitiesData);

		for(int i=0; i<m_EntitiesHeader.entities; ++i)
		{
			fwrite(&(m_pEntitiesData[i].flags),sizeof(CSM_DataStream::FlagBuffer),1,pFile);
			fwrite(&(m_pEntitiesData[i].group),sizeof(CSM_DataStream::ID),1,pFile);

			CSM_DataStream::WriteBuffer(m_pEntitiesData[i].properties,pFile);

			CSM_DataStream::WritePosition(m_pEntitiesData[i].x,pFile);
			CSM_DataStream::WritePosition(m_pEntitiesData[i].y,pFile);
			CSM_DataStream::WritePosition(m_pEntitiesData[i].z,pFile);
		}
	}
public:



	~StructuredStream()
	{
		if(!m_bIsClean) Clear();
	}

	void GetVersion(int& major, int& minor)
	{
		major=CSM_DataStream::versionmajor;
		minor=CSM_DataStream::versionminor;
	}

	void Clear()
	{
		//Don't clear unless necessary
		assert(m_bIsClean==false);

		for (int i=0; i<m_GroupHeader.groups; ++i)
		{
			delete m_pGroupData[i].properties;
			m_pGroupData[i].properties=0;
		}
		delete [] m_pGroupData;
		m_pGroupData=0;

		delete [] m_pVisGroupData;
		m_pVisGroupData=0;

		for (int i=0; i<m_LightMapHeader.lightmaps; ++i)
		{
			delete m_pLightMapData[i].pRGBA_Data;
			m_pLightMapData[i].pRGBA_Data=0;
		}
		delete [] m_pLightMapData;

		for (int i=0; i<m_MeshHeader.meshes; ++i)
		{	
			delete m_pMeshData[i].properties;
			m_pMeshData[i].properties=0;

			for (int j=0;j<m_pMeshData[i].surfaces; ++j)
			{
				delete m_pMeshData[i].surfacedata[j].texturename;
				m_pMeshData[i].surfacedata[j].texturename=0;

				delete [] m_pMeshData[i].surfacedata[j].vdata;
				m_pMeshData[i].surfacedata[j].vdata=0;

				delete [] m_pMeshData[i].surfacedata[j].tdata;
				m_pMeshData[i].surfacedata[j].tdata=0;

				delete [] m_pMeshData[i].surfacedata[j].ldata;
				m_pMeshData[i].surfacedata[j].ldata=0;
			}
			delete [] m_pMeshData[i].surfacedata;
			m_pMeshData[i].surfacedata=0;
		}
		delete [] m_pMeshData;
		m_pMeshData=0;

		for (int i=0; i<m_EntitiesHeader.entities; ++i)
		{
			delete m_pEntitiesData[i].properties;
			m_pEntitiesData[i].properties=0;
		}
		delete [] m_pEntitiesData;

		m_bIsClean=true;
	}

	bool LoadFileFormat(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);

		if(!m_bIsClean) Clear();

		fread(&m_Header,sizeof(CSM_DataStream::Header),1,pFile);
		std::cout<<".csm version="<<m_Header.version<<std::endl;

		//File is not CSM format version 5
		assert(m_Header.version==5);
		if(m_Header.version!=5) return false;

		fread(&m_GroupHeader,sizeof(CSM_DataStream::Group_Header),1,pFile);
		if(m_GroupHeader.groups!=0) ReadGroups(pFile);

		fread(&m_VisGroupHeader,sizeof(CSM_DataStream::Vis_Group_Header),1,pFile);
		if (m_VisGroupHeader.groups!=0) ReadVisGroups(pFile);

		fread(&m_LightMapHeader,sizeof(CSM_DataStream::LightMap_Header),1,pFile);
		if (m_LightMapHeader.lightmaps!=0) ReadLightMaps(pFile);

		fread(&m_MeshHeader,sizeof(CSM_DataStream::Mesh_Header),1,pFile);
		if (m_MeshHeader.meshes!=0) ReadMeshes(pFile);

		fread(&m_EntitiesHeader,sizeof(CSM_DataStream::Entities_Header),1,pFile);
		if (m_EntitiesHeader.entities!=0) ReadEntities(pFile);

		long position = ftell(pFile);
		//std::cout<<"CameraDataByteOffset="<<position<<std::endl;

		fread(&m_CameraData,sizeof(CSM_DataStream::Camera_Data),1,pFile);

		//std::cout<<"\tx,y,zCamera="<<m_CameraData.x<<
		//		","<<m_CameraData.y<<","<<
		//		m_CameraData.z<<std::endl;
		//std::cout<<"\tPitch#="<<m_CameraData.pitch<<std::endl;
		//std::cout<<"\tYaw="<<m_CameraData.yaw<<std::endl;

		m_bIsClean=false;

		return true;
	};

	/*
	bool WriteFileFormat(CSM_DataStream::FilePointer pFile)
	{
		assert(pFile);

		fwrite(&m_Header,sizeof(CSM_DataStream::Header),1,pFile);

		fwrite(&m_GroupHeader,sizeof(CSM_DataStream::Group_Header),1,pFile);
		if(m_GroupHeader.groups>0) WriteGroups(pFile);

		fwrite(&m_VisGroupHeader,sizeof(CSM_DataStream::Vis_Group_Header),1,pFile);
		if (m_VisGroupHeader.groups>0) WriteVisGroups(pFile);

		fwrite(&m_LightMapHeader,sizeof(CSM_DataStream::LightMap_Header),1,pFile);
		if (m_LightMapHeader.lightmaps>0) WriteLightMaps(pFile);

		fwrite(&m_MeshHeader,sizeof(CSM_DataStream::Mesh_Header),1,pFile);
		if (m_MeshHeader.meshes>0) WriteMeshes(pFile);

		fwrite(&m_EntitiesHeader,sizeof(CSM_DataStream::Entities_Header),1,pFile);
		if (m_EntitiesHeader.entities>0) WriteEntities(pFile);

		fwrite(&m_CameraData,sizeof(CSM_DataStream::Camera_Data),1,pFile);


		return 1;
	};
	*/
};