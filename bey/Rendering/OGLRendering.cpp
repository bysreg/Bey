#include "OGLRendering.h"
#include "RenderingInitData.h"
#include "OGLBuffer.h"
#include <glew.h>

using namespace bey;

OGLRendering::OGLRendering()
{
}

OGLRendering::~OGLRendering()
{
}

void OGLRendering::Init(const RenderingInitData* data)
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		MessageBox(0, L"glewInit Failed.", 0, 0);
	}

	glViewport(0, 0, data->screenWidth, data->screenHeight);
	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
}

void OGLRendering::Clean()
{
}

void OGLRendering::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRendering::SwapBuffer()
{
}

IBuffer* OGLRendering::CreateBuffer(const BufferDesc* bufferDesc)
{
	BeyNativeBuffer* nativeBuffer = new GLuint;
	GLenum target = BufferDesc::ConvertEBufferType(bufferDesc->type);
	GLenum usage = BufferDesc::ConvertEBufferUsage(bufferDesc->usage);

	glGenBuffers(1, nativeBuffer);
	glBindBuffer(target, *nativeBuffer);
	glBufferData(target, bufferDesc->totalByteSize, bufferDesc->data, usage);
	glBindBuffer(target, 0);

	IBuffer* buffer = new OGLBuffer;
	buffer->Init(nativeBuffer, *bufferDesc);

	return  buffer;
}

void OGLRendering::BindBuffer(const IBuffer& buffer)
{

}

void OGLRendering::Render(const RenderData& renderData)
{

}

IShader* OGLRendering::CompileShader(const CompileShaderData& compileShaderData)
{

}

IInputLayout* OGLRendering::CreateInputLayout(const InputLayoutDesc* inputLayoutDesc, int numInputLayoutDesc, IShader* compiledShader)
{

}

