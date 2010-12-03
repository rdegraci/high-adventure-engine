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

#include "Texture.h"
#include "DataStream.h"
#include "MemoryFrameStack.h"
#include "SurfaceRepr.h"
#include "MemoryFrame.h"
#include "TextureManager.h"
#include "MeshBufferRepr.h"

//These forward declarations are necessary because these templates
//are used as parameters to MemoryFrame and MemoryFrameStack
//within the Polygonal struct

template <typename SurfaceReprTrait,
		  template <typename> class SurfaceStoragePolicy>
class SurfaceRepr;

template <typename MeshReprTrait>
class DisplayList;


struct Polygonal
{
	typedef Texture Texture;

	typedef SurfaceRepr<Polygonal,DisplayList> Surface; 
	typedef DataStream<CSM_DataStream,StructuredStream,Chunked> DataStream;

	typedef int DisplayListID;
	typedef	int TextureObjectID;

	typedef GLfloat VertexComponent;
	typedef GLfloat Position;
	typedef GLfloat VertexData;

	typedef DisplayListID DisplayListIDBuffer;
	typedef TextureObjectID TextureObjectIDBuffer;

	typedef long Count;
	typedef unsigned short Perf_Index;	//Performance Index (16bit for speed)
	typedef Perf_Index	IndexData;
	typedef GLuint Length;
	typedef TextureManager TextureManager;

};


struct ParametricPatch
{
	//TODO: Implement ParametricPatch types
};


