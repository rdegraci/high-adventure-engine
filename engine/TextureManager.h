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
#include "Texture.h"
#include "glut-3.7.6-bin/Glut.h"


#ifndef _TGA_LOADER
#define _TGA_LOADER
//Needs to be defined before tga_loader.h, used by tga_loader.h
#define LOAD_TGA
#include "tga_loader.h"
#endif 

//Loads a texture
//Associates the texture with the texture path

//If the texture is new, then be able to return the texture ID
//If the texture is already loaded, then be able to return the Index



//There is only one texture manager

class Texture;

class TextureManager
{
	std::vector<Texture*> m_TexturePathContainer;
	GLuint*	m_pTextureIDBuffer;

	bool IsStored(std::string strPath, GLuint& ID)
	{
		std::vector<Texture*>::iterator i;
		bool stored(false);

		for(i=m_TexturePathContainer.begin();i!=m_TexturePathContainer.end();++i)
		{
			if(strPath==(*i)->GetTexturePath())
			{
				stored=true;
				ID=(*i)->GetTextureID();
				break;
			}
		}
		return stored;
	}

	int LoadTextureObject(std::string strTexturePath, GLuint texturename)
	{
		const char* pszTGAPath=strTexturePath.c_str();
		int textureindex=0;
		SDL_RWops* area=SDL_RWFromFile(pszTGAPath, "rb");

		SDL_Surface* pBitmap(0);
		pBitmap = IMG_LoadTGA_RW(area);
		assert(pBitmap);

		SDL_FreeRW(area);

		
		glBindTexture(GL_TEXTURE_2D,texturename);
		assert(GL_NO_ERROR==glGetError());

		//TODO: Enable using MipMaps. Always try to use MipMaps.

		//Note: we use GL_BGR_EXT since the TGA loader code loads the tga
		//with BGR color components rather than RGB color components
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->w, pBitmap->h, GL_BGR_EXT, GL_UNSIGNED_BYTE, pBitmap->pixels);

		//GL_LINEAR is the smoothest.    
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);    
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		assert(GL_NO_ERROR==glGetError());


		//Set the SDL_Surface to and openGL texture
		//Note: we use GL_BGR_EXT since the TGA loader code loads the tga
		//with BGR color components rather than RGB color components
		//glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,pBitmap->w,pBitmap->h,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,pBitmap->pixels);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->w, pBitmap->h, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pBitmap->pixels);
		assert(GL_NO_ERROR==glGetError());

		//Free the bitmap data that we loaded since openGL stored it as a texture  
		SDL_FreeSurface(pBitmap);  

		return textureindex;
	}

public:

	TextureManager() : m_pTextureIDBuffer(0)
	{
		m_pTextureIDBuffer=new GLuint[64];
		CreateTextureObjectBuffer();
	}

	~TextureManager()
	{
		ReclaimTextureObjectBuffer();
		delete [] m_pTextureIDBuffer;
		m_pTextureIDBuffer=0;

		std::vector<Texture*>::iterator i;
		bool stored(false);

		for(i=m_TexturePathContainer.begin();i!=m_TexturePathContainer.end();++i)
		{
			delete(*i);
			*i=0;
		}
		m_TexturePathContainer.clear();
	}

	void CreateTextureObjectBuffer()
	{
		assert(m_pTextureIDBuffer);

		glGenTextures(64,m_pTextureIDBuffer);
	}

	void ReclaimTextureObjectBuffer()
	{
		assert(m_pTextureIDBuffer);

		const GLuint* pBuffer(m_pTextureIDBuffer);
		glDeleteTextures(64,pBuffer);
	}

	bool Load(std::string strTexturePath, GLuint& id)
	{
		assert(m_pTextureIDBuffer);

		bool alreadystored=false;

		if(!IsStored(strTexturePath,id))
		{
			Texture* pTexture=new Texture;
			pTexture->SetTexturePath(strTexturePath);
			m_TexturePathContainer.push_back(pTexture);
			size_t texturecount=m_TexturePathContainer.size();
			id=m_pTextureIDBuffer[texturecount];
			pTexture->SetTextureID(id);
			LoadTextureObject(strTexturePath,id);
			alreadystored=false;
		}
		else
		{
			//id has been changed by the call to IsStored
			alreadystored=true;
		}
		return alreadystored;
	}

};

