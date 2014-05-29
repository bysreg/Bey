#pragma once

#include "Common\DataTypeDefs.h"

namespace bey {

	enum E_SHADER_TYPE {
		E_VERTEX_SHADER = 0,
		E_FRAGMENT_SHADER,
	};

	struct CompileShaderData {
		BeyChar* filepath;
		E_SHADER_TYPE shaderType;
	};

	struct ShaderInitData {		
		E_SHADER_TYPE shaderType;

	};

	class IShader {		
		virtual void Init(const ShaderInitData& data) = 0;
		virtual void Clean() = 0;		
	};

}