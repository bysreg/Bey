#pragma once

#include "Common\DataTypeDefs.h"

//forward declarations
#ifdef DIRECT3D
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
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
		ID3DBlob* nativeProgram; // TODO : isn't blob only used to create the shader object. after that, it's useless right ? if yes, then we shouldn't store it
		ID3D11VertexShader* vertexShaderObject; // only used if shaderType is E_VERTEX_SHADER
		ID3D11PixelShader* fragmentShaderObject; // we use fragment keyword here. only used if shaderType is E_FRAGMENT_SHADER
#endif
	};

	class IShader {
	public:
		virtual void Init(const ShaderInitData& data) = 0;
		virtual void Clean() = 0;	

		//ctor, dtor
		IShader() {};
		virtual ~IShader() {};
	};

}