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

template <typename SceneGraphNodeTrait=StandardSceneGraph,
		  template <typename> class ContainmentPolicy=BoundingSphere>
class Leaf : public Component<SceneGraphNodeTrait>,
			 public ContainmentPolicy<SceneGraphNodeTrait>
{
public:

	Leaf()
	{}

	virtual ~Leaf()
	{
		//We do not delete m_pModel
		//since the extent of the Model may outlive this Leaf
	}


	Leaf(typename SceneGraphNodeTrait::DrawableObject* pModel,Component<SceneGraphNodeTrait>* pParent)
	{
		assert(pModel);
		assert(pParent);

		m_pModel=pModel;
		m_pParent=pParent;
	}

	virtual void UpdateWorldBound()
	{
		//TODO:
		//The model bounding region is assumed to be correct, however if the model's mesh has changed
		//then we will need to update the bounding region
		//
		//	if(m_pModel->MeshChanged())
		//	{
		
				//Composite<SceneGraphNodeTrait>::ComputeContainingAxisAlignedBox(m_pModel->GetMeshVertexBuffer());

		//	}
		//	else
		//	{
		//	}
		//
		m_BoundingRegion=ContainmentPolicy<SceneGraphNodeTrait>::ComputeContainingAxisAlignedBox(m_pModel->GetMeshVertexBuffer());
	}


	//Operations

	virtual void Display(const typename SceneGraphNodeTrait::Renderer& renderer) const
	{
		renderer.AddToSortBuffer(m_pModel,m_LocalS,m_LocalR,m_LocalT);
	}
};