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

#include "Vector.h"


template <typename T,
		  typename BoundingSphereCullTrait>
class BoundingSphereCull
{

public:

	//Test bounding region against a plane
	bool IsCulled(const typename BoundingSphereCullTrait::BoundingRegion& br,
				  const typename BoundingSphereCullTrait::Plane& plane) const;
};

template<typename T,
		 typename BoundingSphereCullTrait>
bool BoundingSphereCull<T,BoundingSphereCullTrait>::IsCulled(const typename BoundingSphereCullTrait::BoundingRegion& br, 
															 const typename BoundingSphereCullTrait::Plane& plane) const
{

	//If the following is true
	// (N dot C) + D <= (-r)
	// then the bounding region is completely culled
	//(N dot C) - D is the signed distance from the center of the sphere to the plane
	//C is the center of the Sphere
	//D is the D of the plane equation Ax+By+Cz+D=0
	//r is the radius of the Sphere

	//Verify that the Plane data is correct
	assert( fabs((Vector<float,3,1>::Dot(plane.m_N,plane.m_P) + plane.GetD())) < 0.001f);	//zero

	return ((Vector<T,3,1>::Dot(plane.m_N,br.m_C))+plane.GetD()<=-br.m_R);
}
