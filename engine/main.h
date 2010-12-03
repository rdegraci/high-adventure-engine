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

//main.h




#include "DataStream.h"
#include "Component.h"
#include "Composite.h"
#include "Leaf.h"
#include "RendererRepr.h"
#include "Model.h"

//Data Streams
typedef DataStream<CSM_DataStream,StructuredStream, Chunked> CSM_Stream;

//Scene Graph
typedef Component<StandardSceneGraph> SceneGraphComponent;
typedef Composite<StandardSceneGraph,BoundingSphere> SceneGraphComposite;
typedef Leaf<StandardSceneGraph> SceneGraphLeaf;

//Rendering
typedef RendererRepr<float,
					 HardwareRendering,
					 OpenGLViewFrustum,
					 Accelerated,
					 BoundingSphereCull> Renderer;

//Surfaces and Meshes 
typedef SurfaceRepr<Polygonal,DisplayList> PolygonalSurface;

typedef GLuint TextureObjectID;
typedef GLuint DisplayListID;

//Storage
typedef std::vector<PolygonalSurface*> PolygonalSurfaceStorage;
typedef std::vector<Model*> ModelStorage;


int main(int argc, char* argv[] );
bool InitOpenGLWindow();
bool InitSDLSubSystems();
void StartGameEngine();
void CalcFramesPerSecond();
void EngineTests();

void ReclaimModelStorage(ModelStorage& modelstorage);
void BuildDynamicModelStorage(ModelStorage& modelstorage,CSM_Stream& levelstream,TextureManager& texturemanager);
void BuildSceneGraph(const ModelStorage& modelstorage, SceneGraphComposite& scenegraph, CSM_Stream& levelstreams);
bool ExtractValue(const std::string& strValueName, const std::string& strString, std::string& strResult);




