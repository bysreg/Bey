#include "GameApp.h"
#include "Rendering\D3DRendering.h"
#include "Rendering\RenderingInitData.h"
#include <cassert>

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
	m_Rendering->Render();
}