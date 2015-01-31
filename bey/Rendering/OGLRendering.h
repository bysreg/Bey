#pragma once

#include "IRendering.h"

namespace bey
{

	class OGLRendering : public IRendering
	{
	public:
		//override interface
		virtual void Init(const RenderingInitData* data);
		virtual void Clean();
		virtual void Clear();
		virtual void SwapBuffer();
		virtual IBuffer* CreateBuffer(const BufferDesc* bufferDesc);
		virtual void BindBuffer(const IBuffer& buffer);
		virtual void Render(const RenderData& renderData);
		virtual IShader* CompileShader(const CompileShaderData& compileShaderData);
		virtual IInputLayout* CreateInputLayout(const InputLayoutDesc* inputLayoutDesc, int numInputLayoutDesc, IShader* compiledShader);

		OGLRendering();
		~OGLRendering();

		//specific OpenGL


	private:

		OGLRendering(OGLRendering const&); // don't implement
		void operator=(OGLRendering const&); // don't implement

	};

}