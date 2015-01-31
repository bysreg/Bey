#pragma once

#include <windows.h>

namespace bey {

	struct RenderingInitData {
		int screenWidth;
		int screenHeight;
#ifdef BEY_OS_WIN32
		HWND handleWindow;
#endif
	};
}