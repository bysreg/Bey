#include "GameApp.h"
#if BEY_USE_DIRECT3D
#include "Rendering\D3DRendering.h"
#elif BEY_USE_OPENGL
#include "Rendering\OGLRendering.h"
#endif
#include "Rendering\IInputLayout.h"
#include "Rendering\RenderingInitData.h"
#include "Rendering\Vertex.h"
#include "Rendering\BufferDesc.h"
#include "Rendering\IBuffer.h"
#include "Rendering\IShader.h"
#include "Rendering\InputLayoutDesc.h"
#include <d3d11.h>
#include <cassert>
#include <sstream>

using namespace bey;

GameApp::GameApp() :
m_Width(0),
m_Height(0),
m_Hwnd(),
m_Rendering(nullptr),
m_VertexBuffer(nullptr),
m_IndexBuffer(nullptr),
m_RenderData()
{
#ifdef BEY_USE_DIRECT3D
	m_Rendering = new D3DRendering;
#elif BEY_USE_OPENGL
	// TODO : fill with open gl
#endif
}

void GameApp::Init()
{
	assert(m_Height > 0 && m_Width > 0);
	assert(m_Hwnd != nullptr);

	RenderingInitData data;
	data.screenHeight = m_Height;
	data.screenWidth = m_Width;
	data.handleWindow = m_Hwnd;

	m_Rendering->Init(&data);
}

void GameApp::Init(int width, int height, HWND hWnd)
{
	m_Width = width;
	m_Height = height;
	m_Hwnd = hWnd;

	Init();

	//colors
	const RGBA_NORM GREEN = { 0.0f, 1.0f, 0.0f, 1.0f };
	const RGBA_NORM BLUE = { 0.0f, 0.0f, 1.0f, 1.0f };
	const RGBA_NORM RED = { 1.0f, 0.0f, 0.0f, 1.0f };

	VertexColor vertices[] = {
		{ BeyFloat3(0.0f, 1.0f, 0.5f), GREEN },
		{ BeyFloat3(1.0f, -1.0f, 0.5f), BLUE },
		{ BeyFloat3(-1.0f, -1.0f, 0.5f), RED },
	};

	// vertex buffer desc
	BufferDesc vbd;
	vbd.usage = E_BU_IMMUTABLE;
	vbd.totalByteSize = sizeof(vertices);
	vbd.elementByteSize = sizeof(VertexColor);
	vbd.type = E_BT_VERTEX_BUFFER;
	vbd.data = vertices;

	m_VertexBuffer = m_Rendering->CreateBuffer(&vbd);

	// Create the index buffer
	BeyUint indices[] = {
		0, 1, 2
	};

	//index buffer desc
	BufferDesc ibd;
	ibd.usage = E_BU_IMMUTABLE;
	ibd.totalByteSize = sizeof(indices);
	ibd.elementByteSize = sizeof(BeyUint);
	ibd.type = E_BT_INDEX_BUFFER;
	ibd.data = indices;

	m_IndexBuffer = m_Rendering->CreateBuffer(&ibd);

	CompileShaders();
	CreateVertexLayout();

	m_RenderData.renderType = E_RENDER_TYPE::E_USE_INDEX_BUFFER;
	m_RenderData.vertexBuffer = m_VertexBuffer;
	m_RenderData.vertexCount = sizeof(vertices) / sizeof(VertexColor);
	m_RenderData.vs = m_Vs;
	m_RenderData.fs = m_Fs;
	m_RenderData.inputLayout = m_InputLayout;
	m_RenderData.indexBuffer = m_IndexBuffer;
	m_RenderData.indexCount = sizeof(indices) / sizeof(BeyUint);
}

void GameApp::Clean()
{
	m_InputLayout->Clean();
	delete m_InputLayout;
	m_Fs->Clean();
	delete m_Fs;
	m_Vs->Clean();
	delete m_Vs;
	m_IndexBuffer->Clean();
	delete m_IndexBuffer;
	m_VertexBuffer->Clean();
	delete m_VertexBuffer;
	m_Rendering->Clean();
	delete m_Rendering;
}

void GameApp::Update(float dt)
{
}

void GameApp::Render()
{
	m_Rendering->Clear();	
	m_Rendering->Render(m_RenderData);
	m_Rendering->SwapBuffer();
}

void GameApp::CalculateFps(float dt)
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.

	static int frameCount = 0;
	static float timeElapsed = 0.0f;

	frameCount++;
	timeElapsed += dt;

	if (timeElapsed >= 1.0f)
	{
		float fps = (float)frameCount; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

		std::wostringstream outs;
		outs.precision(6);
		outs << L"FPS: " << fps << L"    "
			<< L"Frame Time: " << mspf << L" (ms)";
		SetWindowText(m_Hwnd, outs.str().c_str());

		// Reset for next average.
		frameCount = 0;
		timeElapsed -= 1.0f;
	}
}

void GameApp::CompileShaders()
{
	CompileShaderData vcsd;
	vcsd.filepath = "..\\res\\shaders\\src\\simple_vert.hlsl";
	vcsd.shaderType = E_SHADER_TYPE::E_VERTEX_SHADER;
	m_Vs = m_Rendering->CompileShader(vcsd);

	CompileShaderData fcsd;
	fcsd.filepath = "..\\res\\shaders\\src\\simple_frag.hlsl";
	fcsd.shaderType = E_SHADER_TYPE::E_FRAGMENT_SHADER;
	m_Fs = m_Rendering->CompileShader(fcsd);
}

void GameApp::CreateVertexLayout()
{
	InputLayoutDesc vertexDesc[] =
	{
		{ E_IL_POSITION, 0, 0 },
		{ E_IL_COLOR, 0, 12 }
	};

	m_InputLayout = m_Rendering->CreateInputLayout(vertexDesc, 2, m_Vs);
}