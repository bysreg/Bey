#include "GameApp.h"
#include "Rendering\D3DRendering.h"
#include "Rendering\D3DInputLayout.h"
#include "Rendering\D3DUtil.h"
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
m_ConstantBuffer(nullptr),
m_RenderData()
{
	m_Rendering = new D3DRendering;
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
	const RGBA_NORM GREEN = { 1.0f, 1.0f, 1.0f, 1.0f };

	VertexColor vertices[] = {
		/*{ BeyFloat3(-1.0f, -1.0f, -1.0f), GREEN },
		{ BeyFloat3(-1.0f, 1.0f, -1.0f), GREEN },
		{ BeyFloat3(1.0f, 1.0f, -1.0f), GREEN },
		{ BeyFloat3(1.0f, -1.0f, -1.0f), GREEN },
		{ BeyFloat3(-1.0f, -1.0f, 1.0f), GREEN },
		{ BeyFloat3(-1.0f, 1.0f, 1.0f), GREEN },
		{ BeyFloat3(1.0f, 1.0f, 1.0f), GREEN },
		{ BeyFloat3(1.0f, -1.0f, 1.0f), GREEN },*/
		{ BeyFloat3(0.0f, 0.5f, 0.0f), GREEN },
		{ BeyFloat3(0.5f, -0.5f, 0.0f), GREEN },
		{ BeyFloat3(-0.5f, -0.5f, 0.0f), GREEN },
		{ BeyFloat3(0.0f, 0.0f, 1.0f), GREEN },
		{ BeyFloat3(1.0f, 0.0f, 1.0f), GREEN },
		{ BeyFloat3(1.0f, 1.0f, 1.0f), GREEN },
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
	//BeyUint indices[] = {
	//	// front face
	//	0, 1, 2,
	//	0, 2, 3,

	//	// back face
	//	4, 6, 5,
	//	4, 7, 6,

	//	// left face
	//	4, 5, 1,
	//	4, 1, 0,

	//	// right face
	//	3, 2, 6,
	//	3, 6, 7,

	//	// top face
	//	1, 5, 6,
	//	1, 6, 2,

	//	// bottom face
	//	4, 0, 3,
	//	4, 3, 7
	//};	
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

	//create constant buffer
	// CreateConstantBuffer(); // TODO : implement this functionality

	CompileShaders();
	CreateVertexLayout();

	m_RenderData.renderType = E_RENDER_TYPE::E_USE_VERTEX_BUFFER_ONLY;
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

void GameApp::CreateConstantBuffer()
{
	// TODO : not complete

	// constant buffer desc
	BufferDesc cbd;
	cbd.usage = E_BU_IMMUTABLE;
	cbd.totalByteSize = sizeof(VertexColor)* 8; // fixme : do not hardcode the size. hard coded in source code. Ideally, we should get the value from looking or doing reflection on the shader raw text code
	cbd.elementByteSize = sizeof(VertexColor); // hard coded in source code. Ideally, we should get the value from looking or doing reflection on the shader raw text code
	cbd.type = E_BT_CONSTANT_BUFFER;
	cbd.data = nullptr;

	m_ConstantBuffer = m_Rendering->CreateBuffer(&cbd);
}