#pragma once

#include <windows.h>

namespace bey {

	struct RenderingInitData {
		int screenWidth;
		int screenHeight;
#ifdef OS_WIN32
		HWND handleWindow;
#endif
	};
}