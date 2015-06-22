#include "OGLRendering.h"
#include "RenderingInitData.h"
#include "OGLBuffer.h"
#include "OGLShader.h"
#include "Common\Log.h"
#include "OGLInputLayout.h"
#include <glew.h>
#include <SDL.h>

using namespace bey;

OGLRendering::OGLRendering()
{
}

OGLRendering::~OGLRendering()
{
}

void OGLRendering::Init(const RenderingInitData* data)
{
	glewExperimental = GL_TRUE;
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
	// TODO : not implemented yet
}

void OGLRendering::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OGLRendering::SwapBuffer()
{
	// TODO : not implemented yet	
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
	BufferDesc bufferDesc;
	buffer.GetDesc(&bufferDesc);
	GLuint* nativeBuffer = buffer.GetNativeBuffer();
	if (bufferDesc.type == E_BT_VERTEX_BUFFER)
	{
		glBindBuffer(GL_ARRAY_BUFFER, *nativeBuffer);
	}
	else if (bufferDesc.type == E_BT_INDEX_BUFFER)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *nativeBuffer);
	}
}

void OGLRendering::Render(const RenderData& renderData)
{
	// TODO : not implemented yet
}

IShader* OGLRendering::CompileShader(const CompileShaderData& compileShaderData)
{
	GLuint shader = 0;
	GLint compiled = 0;
	GLenum type;
	switch (compileShaderData.shaderType)
	{
	case E_SHADER_TYPE::E_VERTEX_SHADER:
		type = GL_VERTEX_SHADER;
		break;
	case E_SHADER_TYPE::E_FRAGMENT_SHADER:
		type = GL_FRAGMENT_SHADER;
		break;
	}

	shader = glCreateShader(type);

	if (shader == 0)
	{
		BEY_LOG("cannot create shader");
		return nullptr; // something wrong
	}

	// Load the shader source
	FILE * pf;
	if (fopen_s(&pf, compileShaderData.filepath, "rb") != 0)
		return NULL;
	fseek(pf, 0, SEEK_END);
	long size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	char * shaderSrc = new char[size + 1];
	fread(shaderSrc, sizeof(char), size, pf);
	shaderSrc[size] = 0;
	fclose(pf);

	glShaderSource(shader, 1, (const char **)&shaderSrc, NULL);
	delete[] shaderSrc;

	// Compile the shader
	glCompileShader(shader);

	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = new char[infoLen];


			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			BEY_LOGF("Error compiling shader <%s>:\n%s\n", compileShaderData.filepath, infoLog);

			delete[] infoLog;
		}

		glDeleteShader(shader);
		return 0;
	}

	OGLShader* oglShader = new OGLShader;

	ShaderInitData sid;
	sid.shaderType = compileShaderData.shaderType;
	sid.nativeProgram = shader;

	oglShader->Init(sid);

	return oglShader;
}

IInputLayout* OGLRendering::CreateInputLayout(const InputLayoutDesc* inputLayoutDesc, int numInputLayoutDesc, IShader* compiledVertexShader, IShader* compiledFragmentShader)
{	
	OGLInputLayout* oglInputLayout = new OGLInputLayout;
	BeyNativeInputLayout* nativeInputLayout = new BeyNativeInputLayout[numInputLayoutDesc];
	OGLShader* oglVs = static_cast<OGLShader*>(compiledVertexShader);
	OGLShader* oglFs = static_cast<OGLShader*>(compiledFragmentShader);

	// link both the vertex shader and fragment shader
	GLuint shaderProgram = LoadProgram(oglVs->GetCompiledShader(), oglFs->GetCompiledShader());

	for (int i = 0; i < numInputLayoutDesc; i++)
	{
		switch (inputLayoutDesc[i].type)
		{
		case E_INPUT_LAYOUT_TYPE::E_IL_POSITION:
			nativeInputLayout[i] = glGetAttribLocation(shaderProgram, "a_position");
			break;
		case E_INPUT_LAYOUT_TYPE::E_IL_COLOR:
			nativeInputLayout[i] = glGetAttribLocation(shaderProgram, "a_color");
			break;
		}
	}

	oglInputLayout->Init(nativeInputLayout, inputLayoutDesc, numInputLayoutDesc);

	return oglInputLayout;
}

// create the shader program object
GLuint OGLRendering::LoadProgram(GLuint compiledVertexShader, GLuint compiledFragmentShader)
{
	GLuint programObject = glCreateProgram();
	GLint link_status = 0;

	if (programObject == 0)
		return 0;

	glAttachShader(programObject, compiledVertexShader);
	glAttachShader(programObject, compiledFragmentShader);
	glLinkProgram(programObject);
	glGetProgramiv(programObject, GL_LINK_STATUS, &link_status);

	if (link_status == GL_FALSE) // if linking failed
	{
		GLint infoLogLength = 0;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLogLength);

		if (infoLogLength > 1)
		{
			char* infoLog = new char[sizeof(char) * infoLogLength];

			glGetProgramInfoLog(programObject, infoLogLength, NULL, infoLog);
			BEY_LOGF("Error linking program:\n%s\n", infoLog);

			delete infoLog;
		}

		glDeleteProgram(programObject);
		return 0;
	}

	return programObject;
}
