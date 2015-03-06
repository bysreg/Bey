#pragma once

#include "IShader.h"

namespace bey
{
	class OGLShader : public IShader
	{
	public:
		//override interface
		virtual void Init(const ShaderInitData& data);
		virtual void Clean();

		GLuint GetCompiledShader();

		OGLShader();
		~OGLShader();
	private:
		ShaderInitData* m_Data;

	};
}