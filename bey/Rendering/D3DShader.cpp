#include "D3DShader.h"
#include "ShaderInitData.h"
#include <d3d11.h>
#include <D3Dcommon.h>

using namespace bey;

D3DShader::D3DShader() : 
m_Blob(nullptr), 
m_Data(nullptr)
{
}

D3DShader::~D3DShader()
{
}

void D3DShader::Init(const ShaderInitData& data)
{
	m_Data = new ShaderInitData(data);
	m_Blob = data.nativeProgram;
}

void D3DShader::Clean()
{
	if (m_Blob != nullptr) {
		m_Blob->Release();
	}
	delete m_Data;
}

ID3DBlob* D3DShader::GetCompiledShader()
{
	return m_Blob;
}