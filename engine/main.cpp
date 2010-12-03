#include "main.h"

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

#include "main.h"

#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <crtdbg.h>

#include "CreateFlatFloor.h"
#include "Vector.h"
#include "Sphere.h"
#include "MemoryManager.h"
#include "MemoryFrameStack.h"
#include "TextureManager.h"
#include "OpenGLExtension.h"
#include "Model.h"

#ifdef _CRTDBG_MAP_ALLOC
#define new new( _NORMAL_BLOCK, __FILE__, __LINE__)
#endif


int main(int argc, char* argv[] )
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG)|_CRTDBG_LEAK_CHECK_DF); 

	std::cout<<"Initializing SDL Video"<<std::endl;
	int errcode=SDL_Init(SDL_INIT_VIDEO);

	switch (errcode)
	{
	case 0:	
		{
			std::cout<<"Initializing SDL Subsystems"<<std::endl;
			if(InitSDLSubSystems()) 
			{
				std::cout<<"Initializing OpenGL Window"<<std::endl;
				if (InitOpenGLWindow()) 
				{
					std::cout<<"Starting Game Engine"<<std::endl;
					StartGameEngine();
				}
				else std::cerr<<"Unable to initialize OpenGL Windows."<<std::endl;
			}
			else std::cerr<<"Unable to initialize SDL Subsystems"<<std::endl;
			SDL_Quit();
		}
		break;
	case -1:
			std::cerr<<"Unable to start SDL."<<std::endl;
		break;
	};

	return errcode;
}


void StartGameEngine()
{
	SDL_Event event;
	enum ENGINE {	ERR=0, 
					INIT, 
					INTRO, 
					STARTCONSOLE, 
					INGAMECONSOLE, 
					DEBUG, 
					PLAY,
					NEXTLEVEL,
					DIE,
					EXIT } EngineState;



	//Game Data

	OpenGLExtension extensions;
	extensions.Load();
	
	SceneGraphComposite scenegraph;
	CreateFlatFloor floor;
	Renderer renderer;

	CSM_Stream level_stream;
	level_stream.Open("csm_files/test-box.csm");
	level_stream.Read();

	ModelStorage modelstorage;
	TextureManager texturemanager;
	texturemanager.CreateTextureObjectBuffer();

	BuildDynamicModelStorage(modelstorage,level_stream,texturemanager);
	BuildSceneGraph(modelstorage,scenegraph,level_stream);

	EngineState=INIT;

	bool loop=true;
	while(loop)
	{
		switch (EngineState)
		{
		case INIT:
			{
				std::cout<<"Initializing Game Engine"<<std::endl;
				
				EngineState=INTRO;

				floor.Init();
			}
			break;
		case ERR:
			{
				exit(1);
			}
			break;
		case INTRO:
			{
				EngineState=STARTCONSOLE;
			}
			break;
		case STARTCONSOLE:
			{
				EngineState=INGAMECONSOLE;
			}
			break;
		case INGAMECONSOLE:
			{
				EngineState=NEXTLEVEL;
			}
			break;
		case DEBUG:
			{
				bool bBreak=false;
				while(!bBreak&&SDL_PollEvent(&event))
				{
					switch (event.type)
					{
						case SDL_KEYDOWN:
						switch( event.key.keysym.sym )
						{
							case SDLK_ESCAPE:
								EngineState=EXIT;
								goto exitengine;
								break;
							case SDLK_SPACE:
 								EngineState=EXIT;
								goto exitengine;
							break;
							default:
								{
									//push the event back on the queue
									//then break out of this loop
									SDL_Event restore_event;
									restore_event=event;
									SDL_PushEvent(&restore_event);
									bBreak=true;
								}
						};
						break;
						case SDL_QUIT:
							EngineState=EXIT;
							goto exitengine;
						break;
						default:
							{
								//push the event back on the queue
								//then break out of this loop
								SDL_Event restore_event;
								restore_event=event;
								SDL_PushEvent(&restore_event);
								bBreak=true;
							}
					};
				}

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer			
				renderer.CameraClick();

#ifdef _DEBUG
				//EngineTests();
#endif
				//floor.Render();
				//ai.Update();
				//ai.Process();
				scenegraph.Update(1.0f,true);
				scenegraph.Display(renderer);
			}
			break;
		case PLAY:
			{
			}
			break;
		case NEXTLEVEL:
			{
				EngineState=DEBUG;
			}
			break;
		case DIE:
			{
				EngineState=PLAY;
			}
			break;
exitengine:	case EXIT:
			{
				ReclaimModelStorage(modelstorage);
				loop=false;
			}
			break;
		};

		SDL_GL_SwapBuffers();
		CalcFramesPerSecond();
	}
}


// Calculate our FPS
void CalcFramesPerSecond()
{
	char title[255];
	static GLint lasttime=0;
	static GLint Frames = 0;

	//Get number of milliseconds since the SDL library initialization
	GLint currenttime=SDL_GetTicks();

	float deltatime=static_cast<float>(currenttime-lasttime);
	++Frames;

	if(deltatime>=1000.0f)
	{
		float seconds=(deltatime)/1000.0f;
		GLfloat fps =Frames / seconds;
		sprintf(title,"%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);
		SDL_WM_SetCaption(title,title);	

		Frames=0;
		lasttime=currenttime;
	}
}

bool InitOpenGLWindow()
{
	bool flag=false;


	//Videomode 
	bool fullscreen=false;
	bool doublebuffer=true;	
	int  width = 800;
	int height = 600;
	int redbitsize=5;
	int greenbitsize=5;
	int bluebitsize=5;
	int depthbuffersize=16;

	const SDL_VideoInfo* info=SDL_GetVideoInfo();
	assert(info);

	if(info==NULL)
	{
		std::cout<<"Unable to get SDL video information."<<std::endl;
	}
	else 
	{				
		int bpp=info->vfmt->BitsPerPixel;
	
		//Colordepth
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE,redbitsize);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,greenbitsize);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,bluebitsize);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,depthbuffersize);

		//Animation
		int db=0;
		if (doublebuffer==true) db=1;
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,db);


		int flags=SDL_OPENGL;
		if(fullscreen) flags|= SDL_FULLSCREEN;

		//pSurface pointer is freed by SDL_Quit();
		SDL_Surface* pSurface=SDL_SetVideoMode(width,height,bpp,flags);

		if(pSurface) flag=true;	//no error
		else std::cout<<"Unable to sdet SDL video mode."<<std::endl;
	}

	//Projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	return flag;
}

bool InitSDLSubSystems()
{
	//Crappy function call, returns multiple error codes
	//there must be a better way.

	int errcode=0;

	//TODO Parameterize via Text file
	int errtimer=SDL_InitSubSystem(SDL_INIT_TIMER);
	switch (errtimer)
	{
	case 0: std::cout<<"Initializing SDL Timer Subsystem"<<std::endl;
		break;
	case -1: 
		std::cerr<<"Unable to init SDL Timer Subsystem."<<std::endl;
		errcode+=1;
		break;
	};

	int erraudio=SDL_InitSubSystem(SDL_INIT_AUDIO);
	switch (erraudio)
	{
	case 0: std::cout<<"Initializing SDL Audio Subsystem"<<std::endl;
		break;
	case -1: 
		std::cerr<<"Unable to init SDL Audio Subsystem."<<std::endl;
		errcode+=2;
		break;
	};

	int errcdrom=SDL_InitSubSystem(SDL_INIT_CDROM);
	switch (errcdrom)
	{
	case 0: std::cout<<"Initializing SDL CDROM Subsystem"<<std::endl;
		break;
	case -1: 
		std::cerr<<"Unable to init SDL CDROM subsystem."<<std::endl;
		errcode+=4;
		break;
	};

	int errjoystick=SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	switch (errjoystick)
	{
	case 0: std::cout<<"Initializing SDL Joystick Subsystem"<<std::endl;
		break;
	case -1: 
		std::cerr<<"Unable to init SDL Joystick subsystem."<<std::endl;
		errcode+=8;
		break;
	};

	if(!errcode) return true; else return false;
}




void EngineTests()
{	
	//Vector Tests
	Vector3x1<float> P(1.0f,3.0f,5.0f);
	Vector3x1<float> Q(7.0f,11.0f,13.0f);
	Vector3x1<float> R(17.0f,19.0f,23.0f);
	Vector3x1<float> A,B,C;
	float a(0.0f),b(0.0f),c(0.0f);

	//Vector Scalar Tests 1.1
	assert((P+Q)==(Q+P));
	assert((P+Q)+R==P+(Q+R));
	assert(((2*3)*P)==(2*(3*P)));
	assert((2*(P+Q))==(2*P+2*Q));
	assert(((2+3)*P)==(2*P+3*P));

	//Magnitude Tests 1.2
	assert(P.Magnitude()>=0);
	if (P.x==0 && P.y==0 && P.z==0) { assert(P.Magnitude()==0); }
	A=(fabs(-2.0f)*P);
	a=fabs(-2.0f)*P.Magnitude();
	b=A.Magnitude();
	assert(b==a);

	A=P+Q;
	a=A.Magnitude();
	b=P.Magnitude()+Q.Magnitude();
	assert(a<=b);

	
	//Dot Product Tests 1.5
	assert((Vector<float,3,1>::Dot(P,Q))==(Vector<float,3,1>::Dot(Q,P)));
	assert((Vector<float,3,1>::Dot((2*P),Q))==(2*(Vector<float,3,1>::Dot(Q,P))));

	a=Vector<float,3,1>::Dot(P,(Q+R));
	b=(Vector<float,3,1>::Dot(P,Q)+Vector<float,3,1>::Dot(P,R));
	assert((a)==(b));
	
	a=Vector<float,3,1>::Dot(P,P);
	b=P.Magnitude()*P.Magnitude();
	assert((a)==(b));

	a=(abs(Vector<float,3,1>::Dot(P,Q)));
	b=(P.Magnitude()*Q.Magnitude());
	assert(a<=b);


	//Cross Product Tests 1.7
	a=Vector<float,3,1>::Dot((Cross<float,3,1>(P,Q)),P);
	assert(a==0);

	a=Vector<float,3,1>::Dot((Cross<float,3,1>(P,Q)),Q);
	assert(a==0);

	//Cross Product Tests 1.9
	A=Cross<float,3,1>(Q,P);
	B=-(Cross<float,3,1>(P,Q));
	assert(A==B);

	A=Cross<float,3,1>((2*P),Q);
	B=2*(Cross<float,3,1>(P,Q));
	assert(A==B);

	A=Cross<float,3,1>(P,(Q+R));
	B=(Cross<float,3,1>(P,Q)+Cross<float,3,1>(P,R));
	assert(A==B);

	A=Cross<float,3,1>(P,P);
	assert(A.x==0 && A.y==0 && A.z==0);

	a=Vector<float,3,1>::Dot((Cross<float,3,1>(P,Q)),R);
	b=Vector<float,3,1>::Dot((Cross<float,3,1>(R,P)),Q);
	c=Vector<float,3,1>::Dot(Cross<float,3,1>(Q,R),P);
	assert(a==b);
	assert(b==c);

	Matrix<float,3,3> ZZ;
	Matrix<float,3,3> F,G,H;
	Matrix<float,3,3> AA,BB,CC,DD,EE;

	//Matrix Tests 2.1
	AA=F+G;
	BB=G+F;
	assert(AA==BB);

	AA=(F+G)+H;
	BB=F+(G+H);
	assert(AA==BB);

	AA=2.0f*(3.0f*F);
	BB=(2.0f*3.0f)*F;
	assert(AA==BB);

	AA=2.0f*(F+G);
	BB=(2.0f*F)+(2.0f*G);
	assert(AA==BB);

	AA=(2.0f+3.0f)*F;
	BB=(2.0f*F)+(3.0f*F);
	assert(AA==BB);

	//Matrix Tests 2.2
	a=5.0f;
	Matrix<float,2,3> F1;
	F1.m_Storage.M[1][1]=3;
	F1.m_Storage.M[2][2]=5;
	Matrix<float,3,5> G1;
	G1.m_Storage.M[1][1]=3;
	G1.m_Storage.M[2][2]=5;
	Matrix<float,5,7> H1;
	H1.m_Storage.M[1][1]=3;
	H1.m_Storage.M[2][2]=5;
	
	Matrix<float,2,5> AAA=(a*F1)*G1;
	Matrix<float,2,5> BBB=a*(F1*G1);
	assert(AAA==BBB);
	assert(((F1*G1)*H1)==(F1*(G1*H1)));
	
	AAA=(F1*G1);
	Matrix<float,5,2> AAAA=Transpose<float,2,5>(AAA);
	Matrix<float,5,2> BBBB=(Transpose<float,3,5>(G1)*Transpose<float,2,3>(F1));
	assert(AAAA==BBBB);

	//Matrix Inverse Test 2.3
	//Test for Invertibility
	AA=F;
	F.Invert();
	BB=F;
	CC.SetIdentity();
	assert(AA*BB==CC);
	
	//Matrix Inverse Test 2.11
	AA=F*G;
	F.Invert();
	G.Invert();
	BB=F*G;
	assert(AA==BB);

	//TODO: Determinant Tests


	//Transformation Tests
	Matrix3x3<float> I;
	Vector3x1<float> V(1,3,5);
	Vector3x1<float> VA=V;
	V=I*V;
	assert(VA==V);
	
	float c1=3;
	Vector3x1<float> U9(3,5,7);
	Vector3x1<float> V8(11,13,17);
	Matrix3x3<float> m3x3;
	Vector3x1<float> xxx=((c1*U9)+V8);
	Vector3x1<float> a99=(m3x3*xxx);
	Vector3x1<float> b99=(c1*m3x3*U9+m3x3*V8);
	assert(a99==b99);
	
	Rotation<float,XAxis,Real3> Rot1(180);


	typedef Matrix<float,4,4> Matrix4;

	Matrix4 ma;
	ma.m_Storage.M[0][0]=11.0f;ma.m_Storage.M[0][1]=12.0f;ma.m_Storage.M[0][2]=13.0f;
	ma.m_Storage.M[1][0]=21.0f;ma.m_Storage.M[1][1]=22.0f;ma.m_Storage.M[1][2]=23.0f;
	ma.m_Storage.M[2][0]=31.0f;ma.m_Storage.M[2][1]=32.0f;ma.m_Storage.M[2][2]=33.0f;
	ma.Transpose();
	
	typedef Vector<float,1,4> Vector4;
	Matrix4 mm;
	Vector4 vv,ww;
	vv=vv*mm;

	Vector3x1<float> rrv(1,3,5);
	rrv.Normalize();
	Rotation<float,ArbitraryAxis,Real3> rotationmatrix(rrv,90.0);
	float aor=0.0;
	Vector3x1<float> rv;
	RotationMatrixToAngleAxis<float,Real3,RealNumberSystem>(rotationmatrix,rv,aor);
	assert(rv==rrv);
	//assert(aor==90.0);


	//Sphere to Plane test
	Vector3x1<float> center(2,2,2);
	Vector3x1<float> planepoint(0,0,0);
	Vector3x1<float> planenormal(0,3,0);
	Vector3x1<float> planenormalunitized(planenormal);
	planenormalunitized.Normalize();

	typedef Sphere<float,Geometrical> SphereMgr;
	SphereMgr sphere(center,1);	//1 is the radius
	assert(sphere.m_C==center);
	assert(sphere.m_R==1);

	typedef Plane<float,StandardEuclidean>  PlaneMgr;
	PlaneMgr plane(planepoint,planenormal);
	assert(plane.m_P==planepoint);
	assert(plane.m_N==planenormalunitized);


	float ndotc=Vector<float,3,1>::Dot(plane.m_N,sphere.m_C);
	assert(plane.m_P==planepoint);
	assert(plane.m_N==planenormalunitized);


	float signeddistance=ndotc-plane.GetD();
	assert(plane.m_P==planepoint);
	assert(plane.m_N==planenormalunitized);

	float negativer=-sphere.m_R;

	assert( (Vector<float,3,1>::Dot(plane.m_N,plane.m_P) + (plane.GetD()))==0);
	assert(plane.m_P==planepoint);
	assert(plane.m_N==planenormalunitized);

	bool isculled=(signeddistance<negativer);
	assert(isculled==false);


	//Test string extraction
	std::string strTest="\"valuename1\"=\"value1\" \"valuename2\"=\"value2\"";
	std::string strFound;
	bool bfound=ExtractValue("valuename2",strTest,strFound);
	assert(bfound);
	assert(strFound=="value2");
}	


////////	//////////	////////////	//////////////	//////////

void ReclaimModelStorage(ModelStorage& modelstorage)
{
	ModelStorage::iterator i;
	for(i=modelstorage.begin();i!=modelstorage.end();++i)
	{
		delete (*i);
	}
	modelstorage.clear();
}

void BuildDynamicModelStorage(ModelStorage& modelstorage,CSM_Stream& levelstream,TextureManager& texturemanager)
{
	size_t meshcount=levelstream.GetMeshCount();

	//Serialize each mesh, adding each mesh 
	for(size_t i(0);i<meshcount;++i)
	{
		StaticModel* pDynamicModel=new StaticModel;
		pDynamicModel->Create(levelstream,texturemanager,i);
		modelstorage.push_back(pDynamicModel);
		pDynamicModel=0;
	}
}

void BuildSceneGraph(const ModelStorage& modelstorage, SceneGraphComposite& scenegraph,CSM_Stream& levelstream)
{
	//Models may be grouped by using their Group ID
	size_t groups =levelstream.GetTotalGroups();
	if(groups>0)
	{
		//Group IDs start with 1
		for (size_t i=1; i<(groups+1); ++i)
		{
			SceneGraphComponent* pComposite=new SceneGraphComposite();
			for(ModelStorage::const_iterator j=modelstorage.begin();j!=modelstorage.end();++j)
			{
				if(i==(*j)->GetGroupID())
				{	
					SceneGraphComponent* pSceneGraphLeaf=new SceneGraphLeaf((*j),pComposite);
					pComposite->AddChildNode(pSceneGraphLeaf);
				}
				else
				{
					SceneGraphComponent* pSceneGraphLeaf=new SceneGraphLeaf((*j),pComposite);
					scenegraph.AddChildNode(pSceneGraphLeaf);
				}
			}
			scenegraph.AddChildNode(pComposite);
		}
	}
	else
	{
		SceneGraphComponent* pComposite=new SceneGraphComposite();
		for(ModelStorage::const_iterator j=modelstorage.begin();j!=modelstorage.end();++j)
		{
			SceneGraphComponent* pSceneGraphLeaf=new SceneGraphLeaf((*j),pComposite);
			pComposite->AddChildNode(pSceneGraphLeaf);
		}
		scenegraph.AddChildNode(pComposite);
	}
}


/*
void ExtractRoots(const LevelModel& level, Engine3DObjectContainer& eoc)
{
	typedef CSM_Format_V5::Mesh_Header Mesh_Header;
	typedef CSM_Format_V5::Buffer PropertiesString;


	//Engine3DObject* pEngine3DObject=new Engine3DObject(*(level.m_pMeshData));
	//eoc.push_back(pEngine3DObject);

	//pEngine3DObject=new Engine3DObject((level.m_pMeshData[1]));
	//eoc.push_back(pEngine3DObject);

	//pEngine3DObject=new Engine3DObject((level.m_pMeshData[2]));
	//eoc.push_back(pEngine3DObject);

	
	//Extract Level Objects which have parent=null
	//as a property
	//const Mesh_Header& meshheader=level.m_MeshHeader;
	//int meshcount=meshheader.meshes;
	//static const std::basic_string<char>::size_type npos = std::basic_string<char>::size_type();
	//for (int i=0; i<meshcount; ++i)
	//{
	//	PropertiesString* pProperties=level.m_pMeshData[i].properties;
	//	std::basic_string<char>::size_type index1;
	//	index1=pProperties->find("\"parent\"=\"null\"",0);
	//	if(npos!=index1)
	//	{
	//		//Root Object
	//		Engine3DObject* pEngine3DObject=new Engine3DObject(*(level.m_pMeshData));
	//		eoc.push_back(pEngine3DObject);
	//		pEngine3DObject=0;
	//	}
	//	pProperties=0;
	//}
	
}
*/


bool ExtractValue(const std::string& strValueName, const std::string& strString, std::string& strResult)
{
	//strString is of the form "\"name1\"=\"value1\" \"name2\"=\"value2\""
	//strValuename is of the form "name1"

	static const std::basic_string<char>::size_type npos = -1;
	std::string strSearch="\""+strValueName+"\"=\"";
	std::basic_string<char>::size_type length=strSearch.size();
	std::basic_string<char>::size_type index=strString.find(strSearch);
	if(index==npos) return false;

	std::string strValueRaw=strString.substr(index+length);
	std::basic_string<char>::size_type index2=strValueRaw.find("\"");
	if(index==npos) return false;
	std::string strValue=strValueRaw.substr(0,index2);
	strResult=strValue;
	return true;
}


/*
void LoadModel(Model<QuakeModel,QuakeAnimation>*& pModel)
{

	typedef Component<QuakeAnimation,QuakeModel,InterpolateFrames> QuakeModelComponent;

	std::cout<<"Loading Model"<<std::endl;
	assert(pModel==0);
	pModel=new Model<QuakeModel,QuakeAnimation>;

	//Read Lower
	std::string s="models/lara_lower.md3";
	QuakeModel::ModelStorageFormat pMD3Model=new MD3Model;
	pMD3Model->ReadHeader(s.c_str());
	pMD3Model->ReadData(s.c_str());
	QuakeModelComponent* pLowerModelComponent=new QuakeModelComponent;
	pLowerModelComponent->SetModel(pMD3Model);
	pMD3Model=0;
	pLowerModelComponent->SetGameModelID(QuakeModel::LEGS);
	LoadAnimationCFG ("models/lara_animation_lower.cfg",pLowerModelComponent);
	int nTextureID=g_TextureManager.LoadMainSkin("models/lara_lower.tga");
	pLowerModelComponent->SetTheTextureID(nTextureID);
	assert(pModel);
	pModel->SetModelComponentRoot(pLowerModelComponent);


	//Read Upper
	s="models/lara_upper.md3";
	pMD3Model=new MD3Model;
	pMD3Model->ReadHeader(s.c_str());
	pMD3Model->ReadData(s.c_str());
	QuakeModelComponent* pUpperModelComponent=new QuakeModelComponent;
	pUpperModelComponent->SetModel(pMD3Model);
	pMD3Model=0;
	pUpperModelComponent->SetGameModelID(QuakeModel::TORSO);
	LoadAnimationCFG("models/lara_animation_upper.cfg",pUpperModelComponent);
	nTextureID=g_TextureManager.LoadMainSkin("models/lara_upper.tga");
	pUpperModelComponent->SetTheTextureID(nTextureID);
	pLowerModelComponent->AddModelComponent(pUpperModelComponent);


	//Read Weapon
	s="models/railgun.md3";
	pMD3Model=new MD3Model;
	pMD3Model->ReadHeader(s.c_str());
	pMD3Model->ReadData(s.c_str());
	QuakeModelComponent* pWeaponModelComponent=new QuakeModelComponent;
	pWeaponModelComponent->SetModel(pMD3Model);
	pMD3Model=0;
	pWeaponModelComponent->SetGameModelID(QuakeModel::WEAPON);	
	nTextureID=g_TextureManager.LoadMainSkin("models/railgun.tga");
	pWeaponModelComponent->SetTheTextureID(nTextureID);	
	pUpperModelComponent->AddModelComponent(pWeaponModelComponent);

}

template<typename AnimationTrait,
		 typename ModelTrait,
		 template<typename,typename> class AnimationPolicy>
void LoadAnimationCFG(std::string cfgfile,Component<AnimationTrait,ModelTrait,AnimationPolicy>* pModelComponent)
{
	// Open the config file
	std::ifstream fin(cfgfile.c_str());

	int startframe,numframes,loopingframes,framespersecond;
	while( fin >> startframe)
	{
		fin >> numframes >> loopingframes >> framespersecond;

		typename AnimationTrait::SequenceData* pAnimationData=new AnimationTrait::SequenceData;
		pAnimationData->startFrame		= startframe;
		pAnimationData->endFrame		= startframe + numframes;
		pAnimationData->loopingFrames	= loopingframes;
		pAnimationData->framesPerSecond = framespersecond;

		pModelComponent->AddAnimation(pAnimationData);
	}
}
*/



