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

#include <vector>

#include "SurfaceRepr.h"
#include "DataStream.h"
#include "SurfaceReprTrait.h"
#include "DataStreamTrait.h"
#include "MemoryFrame.h"
#include "MemoryFrameStack.h"
#include "TextureManager.h"
#include "MeshBufferRepr.h"


struct DynamicGeometry
{
	typedef float VertexComponent;
	typedef float TextureCoordComponent;
	typedef float Position;
	typedef long Index;	
	typedef long Count;

	typedef std::vector<VertexComponent> VertexComponentBuffer;
	typedef std::vector<Position>	PositionBuffer;
	typedef std::vector<Index>	IndexBuffer;

	typedef SurfaceRepr<Polygonal,VertexArray> Surface; 
	typedef DataStream<CSM_DataStream,StructuredStream,Chunked> DataStream;

	typedef GLfloat VertexData;

	typedef TextureManager TextureManager;
	typedef std::vector<std::string*> TexturePathsContainer;

	typedef unsigned short Perf_Index;	//Performance Index (16bit for speed)
};



struct StaticGeometry
{
	typedef SurfaceRepr<Polygonal,DisplayList> Surface; 
	typedef DataStream<CSM_DataStream,StructuredStream,Chunked> DataStream;

	typedef float VertexComponent;
	typedef float Position;
	typedef long Index;		
	typedef long Count;

	typedef std::vector<VertexComponent> VertexComponentBuffer;
	typedef std::vector<Position>	PositionBuffer;
	typedef std::vector<Index>	IndexBuffer;

	typedef GLuint DisplayListID;
	typedef	GLuint TextureObjectID;
	typedef GLuint Length;
	typedef DisplayListID DisplayListIDBuffer;
	typedef TextureObjectID TextureObjectIDBuffer;
	typedef TextureManager TextureManager;
	typedef std::vector<std::string*> TexturePathsContainer;

};

