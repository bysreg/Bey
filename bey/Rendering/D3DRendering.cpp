#include "D3DRendering.h"
#include "RenderingInitData.h"
#include "D3DUtil.h"
#include <windows.h>
#include <d3d11.h>
#include <assert.h>
#include <cstdio>

using namespace bey;

D3DRendering::D3DRendering() : 
m_SwapChain(NULL), 
m_Device(NULL), 
m_DeviceContext(NULL), 
m_BackBuffer(NULL)
{
}

D3DRendering::~D3DRendering()
{			
}

void D3DRendering::Init(const RenderingInitData* data)
{	
	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDevice(
		NULL,                 // default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,                 // no software device
		createDeviceFlags,
		NULL, NULL,           // default feature level array
		D3D11_SDK_VERSION,
		&m_Device,
		&featureLevel,
		&m_DeviceContext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", 0, 0);
		return;
	}

	if (featureLevel < D3D_FEATURE_LEVEL_11_0)
	{		
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", 0, 0);
		return;
	}

	// Check 4X MSAA quality support for our back buffer format.
	// All Direct3D 11 capable devices support 4X MSAA for all render 
	// target formats, so we only need to check quality support.

	HR(m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_4xMsaaQuality));
	assert(m_4xMsaaQuality > 0);

	// Fill out a DXGI_SWAP_CHAIN_DESC to describe our swap chain.

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc.Width = data->screenWidth; // set the back buffer width
	swapChainDesc.BufferDesc.Height = data->screenHeight; // set the back buffer height	
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1; 
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color mapped to [0, 1] range (NORM)
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // display scanline mode
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; // display scaling mode
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // how swap chain is to be used (draw to back buffer)
	swapChainDesc.BufferCount = 1; // one back buffer
	swapChainDesc.OutputWindow = data->handleWindow;	// the window to be used
	swapChainDesc.Windowed = TRUE;	// windowed/full-screen mode
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // let the display driver select the most efficient presentation method
	swapChainDesc.Flags = 0; // flags. optional

	// Use 4X MSAA? 
	if (m_Enable4xMsaa)
	{
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	// To correctly create the swap chain, we must use the IDXGIFactory that was
	// used to create the device.  If we tried to use a different IDXGIFactory instance
	// (by calling CreateDXGIFactory), we get an error: "IDXGIFactory::CreateSwapChain: 
	// This function is being called with a device from a different IDXGIFactory."

	IDXGIDevice* dxgiDevice = 0;
	HR(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdapter = 0;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

	IDXGIFactory* dxgiFactory = 0;
	HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(m_Device, &swapChainDesc, &m_SwapChain));

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdapter);
	ReleaseCOM(dxgiFactory);	

	// get the address of the back buffer (number 0)
	ID3D11Texture2D *backBuffer;
	HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer));	
	HR(m_Device->CreateRenderTargetView(backBuffer, NULL, &m_BackBuffer)); // use the back buffer address to create the render target
	ReleaseCOM(backBuffer); // destroy the com object used to access the back buffer (the back buffer itself is not destroyed)	

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = data->screenWidth;
	depthStencilDesc.Height = data->screenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // unsigned 24-bit depth buffer mapped to the [0, 1] range with 8 bits uint reserved for stencil buffer mapped to the [0, 255]

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (m_Enable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT; // GPU will be reading and writing to the resource. CPU cannot read or write to resource with this usage. 
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; // CPU will not be reading or writing to the depth/stencil buffer
	depthStencilDesc.MiscFlags = 0;

	HR(m_Device->CreateTexture2D(&depthStencilDesc, 0, &m_DepthStencilBuffer));
	HR(m_Device->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView));
	
	m_DeviceContext->OMSetRenderTargets(1, &m_BackBuffer, m_DepthStencilView); // Bind the render target view and depth/stencil view to the pipeline.

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = (float) data->screenWidth;
	viewport.Height = (float) data->screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_DeviceContext->RSSetViewports(1, &viewport);
}

void D3DRendering::Clean()
{
	m_SwapChain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode, or we wont able to clean D3D

	// close and release all existing COM objects
	m_SwapChain->Release();
	m_BackBuffer->Release();
	m_Device->Release();
	m_DeviceContext->Release();
	m_DepthStencilBuffer->Release();
	m_DepthStencilView->Release();
}

void D3DRendering::Clear()
{
	// clear the back buffer to a deep blue
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_BackBuffer, color);	
}

void D3DRendering::SwapBuffer()
{	
	m_SwapChain->Present(0, 0);
}