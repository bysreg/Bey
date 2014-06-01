#pragma once

#include "Common\DataTypeDefs.h"

namespace bey {

	//forward declarations for namespace bey
	struct RenderingInitData;
	struct BufferDesc;
	class Buffer;
	struct RenderData;
	struct CompileShaderData;
	class IShader;

	class IRendering {
	public:
		virtual void Init(const RenderingInitData* data) = 0;
		virtual void Clean() = 0;				

		// clear default back buffer
		virtual void Clear() = 0;

		// switch the back buffer and the front buffer
		virtual void SwapBuffer() = 0;

		// create a buffer to be bound to the graphics pipeline
		virtual Buffer* CreateBuffer(const BufferDesc* bufferDesc) = 0;

		// bind the buffer to the graphics pipeline
		virtual void BindBuffer(const Buffer& buffer) = 0;

		// render to screen
		virtual void Render(const RenderData& renderData) = 0;

		// compile shader at runtime
		virtual IShader* CompileShader(const CompileShaderData& compileShaderData) = 0;
	};
}