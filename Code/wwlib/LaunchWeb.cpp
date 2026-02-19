/*
**	Command & Conquer Renegade(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/******************************************************************************
*
* FILE
*     $Archive: /Commando/Code/wwlib/LaunchWeb.cpp $
*
* PROGRAMMER
*     Denzil E. Long, Jr.
*     $Author: Denzil_l $
*
* VERSION INFO
*     $Revision: 2 $
*     $Modtime: 6/22/01 4:39p $
*
******************************************************************************/

#include "LaunchWeb.h"
#if defined(OPENW3D_WIN32)
#include <windows.h>
#include <objbase.h>
#include <shellapi.h>
#elif defined(OPENW3D_SDL3)
#include <SDL3/SDL.h>
#endif

/******************************************************************************
*
* NAME
*     LaunchWebBrowser
*
* DESCRIPTION
*     Launch the default browser to view the specified URL
*
* INPUTS
*     URL      - Website address
*
* RESULT
*     Success - True if successful; otherwise false
*
******************************************************************************/
bool LaunchWebBrowser(const char* url)
{
#if defined(OPENW3D_WIN32)
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	// HINSTANCE result = ShellExecuteW(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
	HINSTANCE result = ShellExecuteA(nullptr, "open", url, nullptr, nullptr, SW_SHOWNORMAL);
	CoUninitialize();
	return result >= reinterpret_cast<HINSTANCE>(static_cast<uintptr_t>(32));
#elif defined(OPENW3D_SDL3)
	return SDL_OpenURL(url);
#endif
}
