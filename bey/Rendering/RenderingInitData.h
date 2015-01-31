#pragma once

#ifdef BEY_OS_WIN32
#include <windows.h>
#endif

namespace bey {

	struct RenderingInitData {
		int screenWidth;
		int screenHeight;
#ifdef BEY_OS_WIN32
		HWND handleWindow;
#endif
	};
}