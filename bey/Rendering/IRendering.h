#pragma once

namespace bey {

	//forward declarations for namespace bey
	struct RenderingInitData;

	class IRendering {
	public:
		virtual void Init(const RenderingInitData* data) = 0;
		virtual void Clean() = 0;
		virtual void Render() = 0;
	};
}