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

#include "Model.h"
#include "Component.h"
#include "RendererRepr.h"
#include "Sphere.h"
#include "MeshBufferRepr.h"

template <typename SceneGraphNodeTrait>
class Component;

class Model;

struct StandardSceneGraph
{
	typedef long Count;
	typedef float VertexComponent;
	typedef Model DrawableObject;

	typedef Component<StandardSceneGraph> SceneGraphComponent;
	typedef std::vector<SceneGraphComponent*> ComponentContainer;

	typedef RendererRepr<float,HardwareRendering,OpenGLViewFrustum,Accelerated,BoundingSphereCull> Renderer;

	//Transforms
	typedef Matrix3x3<float> Rotation;
	typedef Matrix3x3<float> Scaling;
	typedef Matrix3x3<float> Translation;

	//Bounding
	typedef Sphere<float,Geometrical> BoundingRegion;

	typedef Vector3x1<float> Point;
	typedef Vector3x1<float> Vertex;
	typedef Vector3x1<float> ColumnVector;
	typedef	float Scalar;

	//Buffer used for Calculating Bounding Volumes
	typedef MeshBufferRepr<VertexBuffer,StructureOfArrays> MeshVertexBuffer;
};