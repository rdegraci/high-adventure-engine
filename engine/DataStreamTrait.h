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

#include <cstdio>
#include <string>
#include <vector>


struct MD3_DataStream
{
};


struct CSM_DataStream
{
	//Cartography Shop Model v5.0

	const static int versionmajor=5;
	const static int versionminor=0;

	typedef FILE* FilePointer;
	
	typedef long Version;
	typedef long Count;
	typedef long FlagBuffer;
	typedef long ID;
	typedef long Color;
	typedef long Boolean;
	typedef long Scalar;
	typedef std::string StringBuffer;
	typedef float Position;
	typedef float VertexComponent;
	typedef long Index;

	typedef struct _Header
	{
		Version version;
	}Header;

	typedef struct _Group_Header
	{
		Count groups;
	}Group_Header;

	typedef struct _Group_Data
	{
		FlagBuffer flags;
		ID id;			
		StringBuffer* properties;
		Color red;		//Object Color
		Color green;	//Object Color
		Color blue;		//Object Color
	}Group_Data;

	typedef struct _Vis_Group_Header
	{
		Count groups;
	}Vis_Group_Header;

	typedef struct _Vis_Group_Data
	{
		Boolean hiddenshown;
		Color red;
		Color green;
		Color blue;
	}Vis_Group_Data;

	typedef struct _LightMap_Header
	{
		Count lightmaps;	//number of lightmaps
	}LightMap_Header;

	typedef struct _LightMap_Data
	{
		Scalar width;
		Scalar height;
		Color* pRGBA_Data;	//width*height*sizeof(Color) 
	}LightMap_Data;


	typedef struct _Mesh_Header
	{
		Count meshes;	//number of meshes
	}Mesh_Header;

	typedef struct _Vertex_Data
	{
		//Per Vertex Data
		Position x;		//Position
		Position y;		//Position
		Position z;		//Position
		Position nx;	//Normal
		Position ny;	//Normal
		Position nz;	//Normal
		Color red;	
		Color green;
		Color blue;
		Position Utexture;		//Texture Coords
		Position Vtexture;		//Texture Coords
		Position Wtexture;		//Texture Coords
		Position Ulightmap;	//Lightmap Coords
		Position Vlightmap;	//Lightmap Coords
		Position Wlightmap;	//Lightmap Coords
	}Vertex_Data;

	typedef struct _Triangle_Data
	{
		Index x;	//Tri Index, x
		Index y;	//Tri Index, y
		Index z;	//Tri Index, z
	}Triangle_Data;

	typedef struct _Line_Data
	{
		Index a;	//Line Index, a
		Index b;	//Line Index, b
	}Line_Data;

	typedef struct _Surface_Data
	{
		FlagBuffer flags;
		StringBuffer* texturename;	//texture name "subdir\texture.jpg"
		ID lightmap;			//Lightmap index (number of lightmaps used for this Surface?)
		Position uoffset;		//texture mapping params
		Position voffset;		//texture mapping params
		Position uscale;		//texture mapping params
		Position vscale;		//texture mapping params
		Position rotation;		//texture mapping params
		Count vertices;			//number of vertices
		Count triangles;		//number of triangles
		Count lines;			//number of lines

		//Dynamic Data Buffers
		Vertex_Data* vdata;		//Vertex data
		Triangle_Data* tdata;	//Tri vertex index
		Line_Data* ldata;		//Edge vertex index

	}Surface_Data;


	typedef struct _Mesh_Data	
	{
		FlagBuffer flags;
		ID group;
		StringBuffer* properties;
		Color red;		//Object Color
		Color green;	//Object Color
		Color blue;		//Object Color
		Position x;		//Center Position
		Position y;		//Center Position
		Position z;		//Center Position
		ID visgroup;
		Count surfaces;	//number of surfaces for this mesh
		
		//Dynamic Data Buffer 
		Surface_Data* surfacedata;	
	}Mesh_Data;


	typedef struct _Entities_Header
	{
		Count entities;	//number of entities
	}Entities_Header;

	typedef struct _Entities_Data
	{
		FlagBuffer flags;
		ID group;	//Entity belongs to this group number
		StringBuffer* properties;
		Position x;	//Position
		Position y;	//Position
		Position z;	//Position
	}Entities_Data;

	typedef struct _Camera_Data
	{
		Position x;	//Camera Position
		Position y;
		Position z;
		Position pitch; //Camera rotation
		Position yaw;
	}Camera_Data;

	// RawStreamData holds the raw data for the entire .CSM file
	typedef struct _RawStreamData
	{
		Header m_Header;
		Group_Header m_GroupHeader;
		Vis_Group_Header m_VisGroupHeader;
		LightMap_Header m_LightMapHeader;
		Mesh_Header m_MeshHeader;
		Entities_Header m_EntitiesHeader;

		//Data Buffers
		Group_Data* m_pGroupData;
		Vis_Group_Data* m_pVisGroupData;
		LightMap_Data* m_pLightMapData;
		Mesh_Data* m_pMeshData;
		Surface_Data* m_pSurfaceData;
		Entities_Data* m_pEntitiesData;

		Camera_Data m_CameraData;
	} RawStreamData;

	//Buffers used for holding temporary data
	typedef std::vector<VertexComponent> VertexComponentBuffer;
	typedef std::vector<Position>	PositionBuffer;
	typedef std::vector<Index>		IndexBuffer;
	typedef std::vector<StringBuffer*>		TexturePathsBuffer;


	//TODO: Move these functions out of here
	static void ReadBuffer(CSM_DataStream::StringBuffer* pBuffer, CSM_DataStream::FilePointer pFile)
	{
		//Quick and dirty read

		pBuffer->clear();
		char c=' ';

		do 
		{
			fread(&c,sizeof(char),1,pFile);
			if (c) *pBuffer=*pBuffer+c;

		} while (c);
	}

	static void WriteBuffer(CSM_DataStream::StringBuffer* pBuffer, CSM_DataStream::FilePointer pFile)
	{
		const char* pString=pBuffer->c_str();
		std::basic_string<char>::size_type length=pBuffer->length();
		if(length>0) fwrite(pString,length,1,pFile);
		
		//Write the terminating zero
		char c=0;
		fwrite(&c,sizeof(char),1,pFile);
	}

	static CSM_DataStream::Position ReadPosition(CSM_DataStream::FilePointer pFile)
	{
		CSM_DataStream::Position f;

		fread(&f,sizeof(CSM_DataStream::Position),1,pFile);

		return f;
	}

	static void WritePosition(CSM_DataStream::Position position, CSM_DataStream::FilePointer pFile)
	{
		CSM_DataStream::Position f=position;

		fwrite(&f,sizeof(CSM_DataStream::Position),1,pFile);

	}
};



