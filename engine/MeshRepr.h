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

#include "MeshReprTrait.h"
#include "StoragePolicy.h"
#include "Model.h"

template <typename MeshReprTrait,
		  template <typename> class StoragePolicy>
class MeshRepr;


template <typename MeshReprTrait,
		  template <typename> class StoragePolicy>
class MeshRepr : public StoragePolicy<MeshReprTrait>
{
public:
	void Serialize()
	{
	}
};


template <>
class MeshRepr<StaticGeometry,DisplayList> : public DisplayList<StaticGeometry>
{
	StaticGeometry::Surface* m_pSurfaces;

public:

	MeshRepr() : m_pSurfaces(0)
	{}

	MeshRepr(const StaticGeometry::DataStream& datastream,
				   StaticGeometry::TextureManager& texturemanager)
	{
		//Store the 'per Surface' data for this Mesh in DisplayLists
		m_pSurfaces=DisplayList<StaticGeometry>::Create(datastream,texturemanager);
	}

	
	void Draw() const 
	{
	/*
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_TEXTURE_2D);

		glFrontFace(GL_CCW);				
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK,GL_LINE);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		glColor3f(1,0,0);
	*/

		//Do Textures
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT,GL_FILL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glColor3f(1.0f,1.0f,1.0f);	
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);


		CallAllDisplayLists();

	}

	~MeshRepr()
	{
		DisplayList<StaticGeometry>::Reclaim();
		delete [] m_pSurfaces;
		m_pSurfaces=0;
	}
};

template <>
class MeshRepr<DynamicGeometry,VertexArray> : public VertexArray<DynamicGeometry>
{
	DynamicGeometry::Surface* m_pSurfaces;

public:

	MeshRepr() : m_pSurfaces(0)
	{
	}

	~MeshRepr()
	{
		Reclaim();
		delete [] m_pSurfaces;
		m_pSurfaces=0;
	}
	MeshRepr(const StaticGeometry::DataStream& datastream,
				   StaticGeometry::TextureManager& texturemanager)
	{
		//Store the 'per Surface' data for this Mesh in DisplayLists
		m_pSurfaces=VertexArray<DynamicGeometry>::Create(datastream,texturemanager);
	}


	void Draw() const
	{
		/*
		//	WireFrame
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);
		glFrontFace(GL_CCW);				
		glPolygonMode(GL_FRONT, GL_LINE);
		glPolygonMode(GL_BACK,GL_LINE);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		glColor3f(1.0f,1.0f,1.0f);
		*/

		//Do Textures
		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
		glPolygonMode(GL_FRONT,GL_FILL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		glColor3f(1.0f,1.0f,1.0f);	
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);


		DrawAllRanges();
		glColor3f(1.0f,1.0f,1.0f);


	}
};


