#include "GameApp.h"
#include "Rendering\D3DRendering.h"
#include "Rendering\RenderingInitData.h"
#include "Rendering\Vertex.h"
#include "Rendering\D3DUtil.h"
#include <d3d11.h>
#include <cassert>
#include <sstream>

using namespace bey;

GameApp::GameApp() :
m_Width(0), 
m_Height(0),
m_Hwnd(),
m_Rendering(nullptr)
{
	m_Rendering = static_cast<IRendering*>(new D3DRendering);
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

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VertexColor) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	
	ID3D11Buffer* vb;
	//HR(m_Rendering->GetDevice());
}

void GameApp::Clean()
{
	m_Rendering->Clean();
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