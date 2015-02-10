#include "OGLShader.h"

using namespace bey;

void OGLShader::Init(const ShaderInitData& data)
{
	m_Data = new ShaderInitData(data);
}

void OGLShader::Clean()
{
	delete m_Data;
}

OGLShader::OGLShader()
{
}

OGLShader::~OGLShader()
{
}