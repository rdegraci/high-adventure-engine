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
#include "Component.h"
#include "SceneGraphTrait.h"
#include "ContainmentPolicy.h"


template <typename SceneGraphNodeTrait=StandardSceneGraph,
		  template <typename> class ContainmentPolicy=BoundingSphere>
class Composite : public Component<SceneGraphNodeTrait>,
				  public ContainmentPolicy<SceneGraphNodeTrait>
{
protected:
	typename SceneGraphNodeTrait::ComponentContainer m_Children;
public:

	Composite()
	{
	}

	virtual ~Composite()
	{
		//Kill the children 
		if(m_Children.size()>0)
		{
			typename SceneGraphNodeTrait::ComponentContainer::iterator i;
			for (i=m_Children.begin(); i!=m_Children.end(); ++i)
			{
				delete (*i);
			}
			m_Children.clear();
		}
	}

	virtual void AddChildNode(Component<SceneGraphNodeTrait>* pComponent) 
	{
		assert(pComponent);
		pComponent->m_pParent=this;
		m_Children.push_back(pComponent);
	}

	virtual void RemoveChildNode(Component<SceneGraphNodeTrait>* pComponent) 
	{
		typename SceneGraphNodeTrait::ComponentContainer::iterator i;
		for (i=m_Children.begin();i!=m_Children.end();++i)
		{
			if (pComponent==(*i))
			{
				delete (*i);
				m_Children.erase(i);
			}
		}
	}

	//Operations

	virtual void UpdateWorldBound()
	{
		//There should always be children
		assert(this->m_Children.size()>0);
		size_t children=m_Children.size();

		if(children>1)	
		{
			//Quick and dirty technique for merging two or more children

			typename SceneGraphNodeTrait::ComponentContainer::iterator i=m_Children.begin();
			assert(*i);
			m_BoundingRegion=(*i)->m_BoundingRegion;

			for(++i;i!=m_Children.end();++i)
			{
				assert(*i);
				m_BoundingRegion=MergeWith((*i)->m_BoundingRegion,m_BoundingRegion);
			}
		}
		else //One child
		{
			typename SceneGraphNodeTrait::ComponentContainer::iterator i(m_Children.begin());
			m_BoundingRegion=MergeWith((*i)->m_BoundingRegion,m_BoundingRegion);	//Merge with single child
		}
	}



	virtual void UpdateWorldData(float time)
	{
		Component<SceneGraphNodeTrait>::UpdateWorldData(time);

		typename SceneGraphNodeTrait::ComponentContainer::iterator i;
		for (i=m_Children.begin(); i!=m_Children.end(); ++i)
		{
 			(*i)->Update(time,false);
		}
	}

	virtual void Display(const typename SceneGraphNodeTrait::Renderer& renderer) const
	{
		if(renderer.IsBoundingRegionCulled(m_BoundingRegion)) return;
		
		//Recursive Display of the children
		for (typename SceneGraphNodeTrait::ComponentContainer::const_iterator i(m_Children.begin()); i!=m_Children.end(); ++i)
		{
			assert(*i);
			(*i)->Display(renderer);
		}
	}
};


