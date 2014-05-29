#include "D3DShader.h"
#include "ShaderInitData.h"
#include <d3d11.h>

using namespace bey;

D3DShader::D3DShader() : m_Blob(NULL)
{
}


D3DShader::~D3DShader()
{
}

void D3DShader::Init(const ShaderInitData& data)
{
	m_Data = new ShaderInitData(data);
}

void D3DShader::Clean()
{
	delete m_Blob;
}

void D3DShader::Compile()
{

}