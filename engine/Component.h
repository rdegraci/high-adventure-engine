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

#include <cassert>
#include "SceneGraphTrait.h"

template <typename SceneGraphNodeTrait=StandardSceneGraph>
class Component
{

public:

	typename SceneGraphNodeTrait::DrawableObject* m_pModel;

	Component<SceneGraphNodeTrait>* m_pParent;

	//SRT-Transform
	typename SceneGraphNodeTrait::Rotation m_WorldR;
	typename SceneGraphNodeTrait::Scaling m_WorldS;
	typename SceneGraphNodeTrait::Translation m_WorldT;

	//SRT-Transform
	//Used for Transforms within the Parent's coordinate system
	typename SceneGraphNodeTrait::Rotation m_LocalR;
	typename SceneGraphNodeTrait::Scaling m_LocalS;
	typename SceneGraphNodeTrait::Translation m_LocalT;

	//Geometry
	//Used for view frustum culling, needs to be public
	//since we may need to merge the leaf and composite 
	//bounding regions
	typename SceneGraphNodeTrait::BoundingRegion m_BoundingRegion;

public:

	Component() : m_pModel(0),
				  m_pParent(0)
	{}

	virtual ~Component()
	{}

	//Geometric State update entry point
	void Update(float time, bool flag)
	{
		UpdateWorldData(time);
		UpdateWorldBound();

		if(flag) PropagateBoundToRoot();
	}

	virtual void UpdateWorldData(float time)
	{
		//TODO: update dynamically changing render state

		//TODO: update dynamically changing local transforms

		//If world transform is not computed by transform controller
		if(m_pParent)
		{
			m_WorldS=m_pParent->m_WorldS*m_LocalS;
			m_WorldR=m_pParent->m_WorldR*m_LocalR;
			m_WorldT=m_pParent->m_WorldT+m_pParent->m_WorldS*(m_pParent->m_WorldR*m_LocalT);
		}
		else
		{
			m_WorldS=m_LocalS;
			m_WorldR=m_LocalR;
			m_WorldT=m_LocalT;
		}

		//We do not update down the tree here because
		//the Composite will update down.
	}

	void PropagateBoundToRoot()
	{
		if(m_pParent)
		{
			m_pParent->UpdateWorldBound();
			m_pParent->PropagateBoundToRoot();
		}
	}

	virtual void UpdateWorldBound()
	{
		//Transform the center of the bounding region
		m_BoundingRegion.m_C=m_WorldR*m_BoundingRegion.m_C;
		m_BoundingRegion.m_C=m_WorldT*m_BoundingRegion.m_C;
		m_BoundingRegion.m_C=m_WorldS*m_BoundingRegion.m_C;
	}

	virtual void AddChildNode(Component<SceneGraphNodeTrait>* pComponent) 
	{
		//Overide in Composite
		assert(0);
	}

	virtual void RemoveChildNode(Component<SceneGraphNodeTrait>* pComponent) 
	{
		//Overide in Composite
		assert(0);
	}

	virtual void Display(const typename SceneGraphNodeTrait::Renderer& renderer) const
	{
		//Overide in Composite
		assert(0);
	}
};