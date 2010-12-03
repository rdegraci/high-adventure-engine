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

#include "SphereTrait.h"

template <typename T,
		  template <typename> class SphereTrait>
class Sphere 
{
public:

	//Direct Access
	typename SphereTrait<T>::Point m_C;		//Center
	typename SphereTrait<T>::Scalar m_R;	//radius

	Sphere() : m_C(SphereTrait<T>::Point()), m_R(SphereTrait<T>::Scalar())
	{
	}

	Sphere(typename SphereTrait<T>::Point c, typename SphereTrait<T>::Scalar r) : m_C(c), m_R(r)
	{
	}
};
