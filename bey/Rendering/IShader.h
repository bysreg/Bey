#pragma once

#include "Common\DataTypeDefs.h"

//forward declarations
#ifdef DIRECT3D
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
#endif

namespace bey {

	enum E_SHADER_TYPE : unsigned int {
		E_VERTEX_SHADER = 0,
		E_FRAGMENT_SHADER,
	};

	struct CompileShaderData {
		BeyChar* filepath;
		E_SHADER_TYPE shaderType;
	};

	struct ShaderInitData {		
		E_SHADER_TYPE shaderType;
#ifdef DIRECT3D
		ID3DBlob* nativeProgram;
#endif
	};

	class IShader {		
		virtual void Init(const ShaderInitData& data) = 0;
		virtual void Clean() = 0;		
	};

}