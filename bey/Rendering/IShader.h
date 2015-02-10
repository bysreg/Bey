#pragma once

#include "Common\DataTypeDefs.h"

//forward declarations
#ifdef BEY_USE_DIRECT3D
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
#elif BEY_USE_OPENGL
#include <glew.h>
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
#ifdef BEY_USE_DIRECT3D
		ID3DBlob* nativeProgram; // contains ID3DBlob from compiling shader, after that it is used to create vertex/fragment shader object
		ID3D11VertexShader* vertexShaderObject; // only used if shaderType is E_VERTEX_SHADER
		ID3D11PixelShader* fragmentShaderObject; // we use fragment keyword here. only used if shaderType is E_FRAGMENT_SHADER
#elif BEY_USE_OPENGL
		GLuint nativeProgram;
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