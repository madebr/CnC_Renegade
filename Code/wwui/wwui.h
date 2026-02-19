#pragma once

#if defined(OPENW3D_WIN32)

#include "AnnounceEvent.h"
#include "AnnounceEvent.h"
#include <windows.h>

namespace UI {

struct WindowMessage {
	HWND hWnd;
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
};

using MessageResult = LRESULT;

using Window = HWND;

}

#elif defined(OPENW3D_SDL3)

#include <SDL3/SDL.h>

namespace UI {

using WindowMessage = SDL_Event;

using MessageResult = Uint32;

using Window = SDL_Window *;

}

#endif
