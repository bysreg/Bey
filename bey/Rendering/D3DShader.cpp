#include "D3DShader.h"
#include "ShaderInitData.h"
#include <d3d11.h>
#include <D3Dcommon.h>

using namespace bey;

D3DShader::D3DShader() : 
m_Blob(nullptr), 
m_Data(nullptr),
m_VertexShaderObject(nullptr), 
m_FragmenShaderObject(nullptr)
{
}

D3DShader::~D3DShader()
{
}

void D3DShader::Init(const ShaderInitData& data)
{
	m_Data = new ShaderInitData(data);
	m_Blob = data.nativeProgram;
	switch (data.shaderType) {
		case E_VERTEX_SHADER :
			m_VertexShaderObject = data.vertexShaderObject;
			break;
		case E_FRAGMENT_SHADER :
			m_FragmenShaderObject = data.fragmentShaderObject;
			break;
	}	
}

void D3DShader::Clean()
{
	if (m_Blob != nullptr) {
		m_Blob->Release();
	}
	delete m_Data;
	m_VertexShaderObject->Release();
	m_FragmenShaderObject->Release();
}

ID3DBlob* D3DShader::GetCompiledShader()
{
	return m_Blob;
}

ID3D11VertexShader* D3DShader::GetVertexShaderObject()
{
	return m_VertexShaderObject;
}

ID3D11PixelShader* D3DShader::GetFragmentShaderObject()
{
	return m_FragmenShaderObject;
}
