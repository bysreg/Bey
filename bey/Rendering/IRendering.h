#pragma once

namespace bey {

	//forward declarations for namespace bey
	struct RenderingInitData;

	class IRendering {
	public:
		virtual void Init(const RenderingInitData* data) = 0;
		virtual void Clean() = 0;				

		// clear default back buffer
		virtual void Clear() = 0;

		// switch the back buffer and the front buffer
		virtual void SwapBuffer() = 0;
	};
}