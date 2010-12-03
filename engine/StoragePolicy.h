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

#include <SDL.h>
#include <SDL_opengl.h>
#include "glext.h"

template <typename StructureOfArraysTrait>
class StructureOfArrays
{
	typename StructureOfArraysTrait::VertexComponentBuffer m_Vertices;

protected:
	StructureOfArrays() 
	{}

public:

	//Public for Speed
	typename StructureOfArraysTrait::ArrayData m_RawComponentData;

	//Operations

	void Reclaim()
	{
		m_RawComponentData.Reclaim();
	}

	typename StructureOfArraysTrait::ArrayData Create(const typename StructureOfArraysTrait::DataStream& datastream)
	{
		typename StructureOfArraysTrait::Count surfacecount=datastream.GetSurfaceCountAtMeshPosition();
		typename StructureOfArraysTrait::Count vertexcount=datastream.GetTotalIndexedVertexCountAtMeshPosition();

		m_RawComponentData.length=vertexcount;
		m_RawComponentData.Create(vertexcount);

		for (typename StructureOfArraysTrait::Count i(0); i<surfacecount; ++i)
		{
			datastream.GetVertexDataPerSurface(m_Vertices);
			size_t totalverts=m_Vertices.size();

			for (size_t j(0);j<totalverts/3;++j)
			{	
				//Insert three vertex components (X,Y,Z)
				m_RawComponentData.Insert(m_Vertices[j*3+0],m_Vertices[j*3+1],m_Vertices[j*3+2],j);
			}
		}

		return m_RawComponentData;
	}

	typename StructureOfArraysTrait::ArrayData Create(typename StructureOfArraysTrait::Count length)
	{
		assert(m_RawComponentData.length==0);

		m_RawComponentData.length=length;
		m_RawComponentData.Create(length);
	}

};


template <typename VertexArrayTrait>
class VertexArray
{
	//We do not own m_pSurfaceBuffer, however we may re-use it within this Policy
	typename VertexArrayTrait::Surface* m_pSurfaceBuffer;
	typename VertexArrayTrait::Count m_SurfaceCount;
	
	//We own the following
	typename VertexArrayTrait::VertexData* m_pVertexData;
	typename VertexArrayTrait::Count m_IndexDataBufferLength;
	typename VertexArrayTrait::Perf_Index* m_pIndexData;
	typename VertexArrayTrait::Perf_Index* m_pPerSurfaceIndexData;
	typename VertexArrayTrait::VertexData* m_pPerSurfaceVertexData;

protected:
	VertexArray() : m_pIndexData(0),
					m_pVertexData(0),
					m_SurfaceCount(0),
					m_pSurfaceBuffer(0),
					m_IndexDataBufferLength(0),
					m_pPerSurfaceVertexData(0),
					m_pPerSurfaceIndexData(0)
	{}

public:

	void DrawAllRanges() const
	{
		glEnable(GL_VERTEX_ARRAY);

		for ( typename VertexArrayTrait::Count i(0); i<m_SurfaceCount; ++i)
		{
			typename VertexArrayTrait::Count rangestart=m_pSurfaceBuffer[i].GetIndexRangeStart();
			typename VertexArrayTrait::Count rangeend=m_pSurfaceBuffer[i].GetIndexRangeEnd();

			//Sometimes there is a null surface (Cartography Shop Models v5.0)
			if(rangeend==0) break;

			//call once to repoint glVertexPointer, glTexCoordPointer, glNormalPointer
			glInterleavedArrays(GL_T2F_N3F_V3F,0,m_pSurfaceBuffer[i].GetVertexData());

			glBindTexture(GL_TEXTURE_2D,m_pSurfaceBuffer[i].GetTextureID());

			/*
			The recommended maximum amounts of vertex and index data can be determined by calling GetIntegerv 
			with the symbolic constants MAX_ELEMENTS_VERTICES and MAX_ELEMENTS_INDICES. If end-start+1 is 
			greater than the value of MAX_ELEMENTS_VERTICES,or if count is greater than the value of 
			MAX_ELEMENTS_INDICES
			*/
			OpenGLExtension::g_pglDrawRangeElements(GL_TRIANGLES,
								rangestart,			
								rangeend,
								rangeend+1,			//Amount of indices in the index array					
								GL_UNSIGNED_SHORT,	//16 bit indices for speed
								m_pIndexData);
		}
	}

	void Reclaim()
	{
		delete [] m_pVertexData;
		m_pVertexData=0;

		delete [] m_pIndexData;
		m_pIndexData=0;
	}

	//One VertexArray per Surface, since each Surface may have a different texture
	typename VertexArrayTrait::Surface* Create(const typename VertexArrayTrait::DataStream& datastream,
											         typename VertexArrayTrait::TextureManager& texturemanager)
	{
		m_SurfaceCount=datastream.GetSurfaceCountAtMeshPosition();
		m_pSurfaceBuffer=new typename VertexArrayTrait::Surface[m_SurfaceCount];

		//totalvertdata=totalmeshverts * ( (two texture coords) + (three normals) + (three vertices) )
		typename VertexArrayTrait::Count totalvertdata(datastream.GetTotalIndexedVertexCountAtMeshPosition()*8);
		m_pVertexData=new typename VertexArrayTrait::VertexData[totalvertdata];

		m_IndexDataBufferLength=datastream.GetTotalTriIndexCountAtMeshPosition();
		m_pIndexData=new typename VertexArrayTrait::Perf_Index[m_IndexDataBufferLength];

		typename VertexArrayTrait::Count rangeend(0);
		typename VertexArrayTrait::Count vertexarrayindex(0);
		typename VertexArrayTrait::Count indexarrayindex(0);

		m_pPerSurfaceVertexData=m_pVertexData;
		m_pPerSurfaceIndexData=m_pIndexData;

		for(typename VertexArrayTrait::Count i(0); i<m_SurfaceCount; ++i)
		{	
			//Load the Texture Paths
			typename VertexArrayTrait::TexturePathsContainer texturepaths;
			datastream.GetTexturePathPerSurface(texturepaths);

			//Set the TextureObjectID
			GLuint id(0);
			texturemanager.Load(*(texturepaths[i]),id);
			m_pSurfaceBuffer[i].SetTextureID(id);	 

			//Load from DataStream into temporary containers
			typename VertexArrayTrait::VertexComponentBuffer normals;
			typename VertexArrayTrait::VertexComponentBuffer vertices;
			typename VertexArrayTrait::PositionBuffer texturecoords;
			typename VertexArrayTrait::IndexBuffer	index;
			datastream.GetNormalDataPerSurface(normals);
			datastream.GetVertexDataPerSurface(vertices);
			datastream.GetTextureCoordDataPerSurface(texturecoords);
			datastream.GetTriIndexPerSurface(index);

			//Insert from temporary containers into the VertexArray and IndexArray
			size_t totalverts=vertices.size();
			vertexarrayindex=0;
			for (size_t j(0);j<totalverts/3;++j)
			{
				//Insert two texture coordinate components (U,V)
				m_pPerSurfaceVertexData[vertexarrayindex+0]=texturecoords[j*2+0];
				m_pPerSurfaceVertexData[vertexarrayindex+1]=texturecoords[j*2+1];

				//Insert three normal components (nX,nY,nZ)
				m_pPerSurfaceVertexData[vertexarrayindex+2]=normals[j*3+0];
				m_pPerSurfaceVertexData[vertexarrayindex+3]=normals[j*3+1];
				m_pPerSurfaceVertexData[vertexarrayindex+4]=normals[j*3+2];
				
				//Insert three vertex components (X,Y,Z)
				m_pPerSurfaceVertexData[vertexarrayindex+5]=vertices[j*3+0];	
				m_pPerSurfaceVertexData[vertexarrayindex+6]=vertices[j*3+1];
				m_pPerSurfaceVertexData[vertexarrayindex+7]=vertices[j*3+2];
				vertexarrayindex+=8;
			}

			size_t totalindices=index.size();
			indexarrayindex=0;
			for (size_t k(0); k<totalindices/3; ++k)
			{
				//Triangle Index is CW, 
				//We will reverse the winding, since the Engine uses CCW
				//Also we cast the indices, since raw indices are 32bit, to 16bit for speed
				m_pPerSurfaceIndexData[indexarrayindex+0]=static_cast<typename VertexArrayTrait::Index>(index[k*3+2]);
				m_pPerSurfaceIndexData[indexarrayindex+1]=static_cast<typename VertexArrayTrait::Index>(index[k*3+1]);
				m_pPerSurfaceIndexData[indexarrayindex+2]=static_cast<typename VertexArrayTrait::Index>(index[k*3+0]);
				indexarrayindex+=3;
			}

			//Set the Surface data
			m_pSurfaceBuffer[i].SetIndexRangeStart(0);	//Usually zero
			m_pSurfaceBuffer[i].SetIndexRangeEnd(indexarrayindex);
			m_pSurfaceBuffer[i].SetVertexData(m_pPerSurfaceVertexData);
			m_pSurfaceBuffer[i].SetIndexData(m_pPerSurfaceIndexData);

			//Bump up the pointers so that they offset into
			//the Vertex Data array and the Index Data array
			m_pPerSurfaceVertexData+=vertexarrayindex;
			m_pPerSurfaceIndexData+=indexarrayindex;
		}

		return m_pSurfaceBuffer;
	}
};





template <typename DisplayListTrait>
class DisplayList
{
	//We do not own m_pSurfaceBuffer, however we may re-use it within this Policy
	typename DisplayListTrait::Surface* m_pSurfaceBuffer;
	typename DisplayListTrait::Count m_SurfaceCount;


	typename DisplayListTrait::DisplayListID m_DisplayListID_Start;
	typename DisplayListTrait::Length m_DisplayListBufferLength;

protected:

	DisplayList() : m_DisplayListBufferLength(0),
					m_DisplayListID_Start(0),
					m_pSurfaceBuffer(0),
					m_SurfaceCount(0)
	{}	

public:

	~DisplayList()
	{
	}

	void Reclaim()
	{
		//Reclaim the Display Lists
		glDeleteLists(m_DisplayListID_Start,m_DisplayListBufferLength);
	}

	typename DisplayListTrait::Surface* GetSurfaceBuffer() const
	{
		return m_pSurfaceBuffer;
	}

	void CallAllDisplayLists() const
	{
		for(typename DisplayListTrait::Count i(0);i<m_SurfaceCount;++i)
		{
			glBindTexture(GL_TEXTURE_2D,m_pSurfaceBuffer[i].GetTextureID());
			glCallList(m_pSurfaceBuffer[i].GetDisplayListID());
		}
	}
	

	typename DisplayListTrait::Surface* Create(const typename DisplayListTrait::DataStream& datastream,
											         typename DisplayListTrait::TextureManager& texturemanager)
	{
		m_SurfaceCount=datastream.GetSurfaceCountAtMeshPosition();
		m_DisplayListBufferLength=m_SurfaceCount;
		m_pSurfaceBuffer=new typename DisplayListTrait::Surface[m_SurfaceCount];
		
		//One DisplayList per Surface, since each Surface may have a different texture
		m_DisplayListID_Start=glGenLists(m_SurfaceCount);


		for (typename DisplayListTrait::Count i(0);i<m_SurfaceCount;++i)
		{
			typename DisplayListTrait::TexturePathsContainer texturepaths;
			datastream.GetTexturePathPerSurface(texturepaths);

			//Load the Texture Paths
			GLuint id(0);
			texturemanager.Load(*(texturepaths[i]),id);

			//Set the TextureObjectID for each Surface
			m_pSurfaceBuffer[i].SetTextureID(id);

			//Set the DisplayListID for each Surface
			m_pSurfaceBuffer[i].SetDisplayListID(m_DisplayListID_Start+i);

			//Build the DisplayList (one DisplayList per Surface)
			//usually efficient if Surfaces contain at least 100 tris each)
			//No need for normals
			typename DisplayListTrait::VertexComponentBuffer vertices;
			typename DisplayListTrait::PositionBuffer texturecoords;
			typename DisplayListTrait::IndexBuffer	index;
			int workingsurface(0);
		
			workingsurface=datastream.GetVertexDataPerSurface(vertices);
			assert(i==workingsurface);

			workingsurface=datastream.GetTextureCoordDataPerSurface(texturecoords);
			assert(i==workingsurface);

			workingsurface=datastream.GetTriIndexPerSurface(index);
			assert(i==workingsurface);

			typename DisplayListTrait::VertexComponent x1,y1,z1,x2,y2,z2,x3,y3,z3;
			typename DisplayListTrait::Position u1,v1,u2,v2,u3,v3;

			size_t indexcount=index.size();
			size_t textcoordcount=texturecoords.size();

			//100 tris minimum for performance
			//assert(indexcount>100);	

			glNewList(m_DisplayListID_Start+i,GL_COMPILE);
			glBegin(GL_TRIANGLES);
			for (size_t j(0);j<indexcount;j+=3)
			{
				x1=vertices[index[j+0]*3+0];
				y1=vertices[index[j+0]*3+1];
				z1=vertices[index[j+0]*3+2];

				x2=vertices[index[j+1]*3+0];
				y2=vertices[index[j+1]*3+1];
				z2=vertices[index[j+1]*3+2];

				x3=vertices[index[j+2]*3+0];
				y3=vertices[index[j+2]*3+1];
				z3=vertices[index[j+2]*3+2];

				u1=texturecoords[index[j+0]*2+0];
				v1=texturecoords[index[j+0]*2+1];
				u2=texturecoords[index[j+1]*2+0];
				v2=texturecoords[index[j+1]*2+1];
				u3=texturecoords[index[j+2]*2+0];
				v3=texturecoords[index[j+2]*2+1];
				

				//Vertices are CW front facing,
				//so reverse for OpenGL, this should be fixed in the 
				//Serialization Policy
				glTexCoord2f(u3,v3);
				glVertex3f(x3,y3,z3);

				glTexCoord2f(u2,v2);
				glVertex3f(x2,y2,z2);

				glTexCoord2f(u1,v1);
				glVertex3f(x1,y1,z1);


			}
			glEnd();
			glEndList();

			vertices.clear();
			texturecoords.clear();
			index.clear();

			texturepaths.clear();
		}

		return m_pSurfaceBuffer; //SurfaceBuffer
	}
};


template <typename VBO_Trait>
class VBO
{
};
