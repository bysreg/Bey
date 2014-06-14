#pragma once

#include "Common\DataTypeDefs.h"

namespace bey {

	//forward declarations for namespace bey
	struct RenderingInitData;
	struct BufferDesc;
	struct InputLayoutDesc;
	struct RenderData;
	struct CompileShaderData;
	class IBuffer;
	class IShader;
	class IInputLayout;

	// class that responsibles for doing graphics pipeline related tasks
	class IRendering {
	public:
		virtual void Init(const RenderingInitData* data) = 0;
		virtual void Clean() = 0;				

		// clear default back buffer
		virtual void Clear() = 0;

		// switch the back buffer and the front buffer
		virtual void SwapBuffer() = 0;

		// create a buffer to be bound to the graphics pipeline
		virtual IBuffer* CreateBuffer(const BufferDesc* bufferDesc) = 0;

		// bind the buffer to the graphics pipeline
		virtual void BindBuffer(const IBuffer& buffer) = 0;

		// render to screen
		virtual void Render(const RenderData& renderData) = 0;

		// compile shader at runtime
		virtual IShader* CompileShader(const CompileShaderData& compileShaderData) = 0;

		// create input layout that feeds vertex data to the graphics pipeline
		virtual IInputLayout* CreateInputLayout(const InputLayoutDesc* inputLayoutDesc, int numInputLayoutDesc, IShader* compiledShader) = 0;

		// TODO : virtual dtor ?
	};
}