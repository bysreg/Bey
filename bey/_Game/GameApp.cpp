#include "GameApp.h"
#include "Rendering\D3DRendering.h"
#include "Rendering\RenderingInitData.h"
#include "Rendering\Vertex.h"
#include "Rendering\D3DUtil.h"
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
m_IndexBuffer(nullptr)
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
	const RGBA_NORM GREEN = {0.0f, 1.0f, 0.0f, 1.0f};	

	VertexColor vertices[] = {
		{ BeyFloat3(-1.0f, -1.0f, -1.0f), GREEN },
		{ BeyFloat3(-1.0f, 1.0f, -1.0f), GREEN },
		{ BeyFloat3(1.0f, 1.0f, -1.0f), GREEN },
		{ BeyFloat3(1.0f, -1.0f, -1.0f), GREEN },
		{ BeyFloat3(-1.0f, -1.0f, 1.0f), GREEN },
		{ BeyFloat3(-1.0f, 1.0f, 1.0f), GREEN },
		{ BeyFloat3(1.0f, 1.0f, 1.0f), GREEN },
		{ BeyFloat3(1.0f, -1.0f, 1.0f), GREEN },
	};	

	// vertex buffer desc
	BufferDesc vbd; 
	vbd.usage = E_BU_IMMUTABLE;
	vbd.totalByteSize = sizeof(VertexColor)* 8;
	vbd.elementByteSize = sizeof(VertexColor);
	vbd.type = E_BT_VERTEX_BUFFER;
	vbd.data = vertices;
	
	m_VertexBuffer = m_Rendering->CreateBuffer(&vbd);
	
	// Create the index buffer
	BeyUint indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};	

	//index buffer desc
	BufferDesc ibd;
	ibd.usage = E_BU_IMMUTABLE;
	ibd.totalByteSize = sizeof(BeyUint)* 36;
	ibd.elementByteSize = sizeof(BeyUint);
	ibd.type = E_BT_INDEX_BUFFER;
	ibd.data = indices;
	
	m_IndexBuffer = m_Rendering->CreateBuffer(&ibd);		

	CompileShaders();
	CreateVertexLayout();
}

void GameApp::Clean()
{
	m_Rendering->Clean();

	if (m_VertexBuffer != nullptr) delete m_VertexBuffer;
	if (m_IndexBuffer != nullptr) delete m_IndexBuffer;
}

void GameApp::Update(float dt)
{
}

void GameApp::Render()
{
	m_Rendering->Clear();	

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
	vcsd.filepath = "..\\res\\shaders\\src\\simpleVS.hlsl";
	vcsd.shaderType = E_SHADER_TYPE::E_VERTEX_SHADER;
	IShader* vs = m_Rendering->CompileShader(vcsd);

	CompileShaderData fcsd;
	fcsd.filepath = "..\\res\\shaders\\src\\simpleFS.hlsl";
	fcsd.shaderType = E_SHADER_TYPE::E_FRAGMENT_SHADER;
	IShader* fs = m_Rendering->CompileShader(fcsd);

	// TODO : save those shaders to class variable 
}

void GameApp::CreateVertexLayout()
{
	// TODO : not yet implemented

	/*InputLayoutDesc vertexDesc[] =
	{		
		{ E_IL_POSITION, 0, 0},
		{ E_IL_COLOR, 0, 12}
	};
	
	m_Rendering
	HR(m_Rendering->->CreateInputLayout(vertexDesc, 2, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout));*/
}