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

template <typename T,
		  typename CameraKeyboardTrait>
class CameraKeyboard
{
protected:

	typename CameraKeyboardTrait::KeyboardFlag panleft;	
	typename CameraKeyboardTrait::KeyboardFlag panright;
	typename CameraKeyboardTrait::KeyboardFlag dollyin;
	typename CameraKeyboardTrait::KeyboardFlag dollyout;
	typename CameraKeyboardTrait::KeyboardFlag trackleft;
	typename CameraKeyboardTrait::KeyboardFlag trackright;


	void HandleKeyBoardInput()
	{
		SDL_Event event;

		while( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
				case SDL_KEYDOWN:
                switch( event.key.keysym.sym )
				{										
					case SDLK_LEFT:
						trackleft=true;
                        break;
                    case SDLK_RIGHT:
						trackright=true;
                        break;
                    case SDLK_UP:
						dollyin=true;
                        break;
                    case SDLK_DOWN:
						dollyout=true;
                        break;
                    default:
                        break;
                }
                break;

				case SDL_KEYUP:
                switch( event.key.keysym.sym )
				{
                    case SDLK_LEFT:
						trackleft=false;
                        break;
                    case SDLK_RIGHT:
						trackright=false;
                        break;
                    case SDLK_UP:
						dollyin=false;
                        break;
                    case SDLK_DOWN:
						dollyout=false;
                        break;
                    default:
                        break;
                }
                break;
				
				case SDL_MOUSEMOTION:
					{
						panleft=(event.motion.x<=10);
						panright=(event.motion.x>=789);	
					}
					break;
				default:
                break;
				
			};

		};	
	}

public:
	
	CameraKeyboard() : panleft(false),
		panright(false),dollyin(false),
		dollyout(false),trackleft(false),
		trackright(false)
	{

	}
};
