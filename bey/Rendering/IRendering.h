#pragma once

#include <windows.h>

namespace bey {

	struct RenderingInitData {
		int screenWidth;
		int screenHeight;
#ifdef WIN32
		HWND handleWindow;
#endif
	};

	class IRendering {
	public:
		virtual void Init(const RenderingInitData& data) = 0;
		virtual void Clean() = 0;
		virtual void Render() = 0;
	};
}