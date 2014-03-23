#include "D3DRendering.h"
#include "RenderingInitData.h"
#include <windows.h>
#include <d3d11.h>
#include <assert.h>

using namespace bey;

IRendering* D3DRendering::m_Instance = nullptr;

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

	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	// clear out the struct for use
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	swapChainDesc.BufferCount = 1;                                    // one back buffer
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	swapChainDesc.BufferDesc.Width = data->screenWidth;                // set the back buffer width
	swapChainDesc.BufferDesc.Height = data->screenHeight;              // set the back buffer height
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used (draw to back buffer)
	swapChainDesc.OutputWindow = data->handleWindow;                   // the window to be used
	swapChainDesc.SampleDesc.Count = 4;                               // how many multisamples (for anti-aliasing, guaranteed support up to 4, minimum 1)
	swapChainDesc.Windowed = TRUE;                                    // windowed/full-screen mode
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;     // allow full-screen switching

	// create a device, device context and swap chain using the information in the swapChainDesc struct
	D3D11CreateDeviceAndSwapChain(
		NULL, // use default adapter (there might be more than one graphics adapter)
		D3D_DRIVER_TYPE_HARDWARE, // use GPU hardware for rendering
		NULL, // used for supplying a software driver. we use null to use hardware for rendering
		NULL, // flags 
		NULL, // feature level list
		NULL, // number of elements in feature level list
		D3D11_SDK_VERSION, // sdk version
		&swapChainDesc, // pointer to pointer to swap chain description struct
		&m_SwapChain, // pointer to pointer to swap chain object
		&m_Device, // pointer to pointer to device object
		NULL, // pointer to feature level variable 
		&m_DeviceContext); // pointer to pointer device context object

	// get the address of the back buffer (number 0)
	ID3D11Texture2D *pBackBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_BackBuffer);
	pBackBuffer->Release(); // destroy the com object used to access the back buffer (the back buffer itself is not destroyed)

	// set the render target as the back buffer
	m_DeviceContext->OMSetRenderTargets(1, &m_BackBuffer, NULL);

	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float) data->screenWidth;
	viewport.Height = (float) data->screenHeight;

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
}

void D3DRendering::Render()
{
	// clear the back buffer to a deep blue
	float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_BackBuffer, color);

	// do 3D rendering on the back buffer here

	// switch the back buffer and the front buffer
	m_SwapChain->Present(0, 0);
}

IRendering& D3DRendering::GetInstance()
{
	if (m_Instance == nullptr) {
		m_Instance = new D3DRendering;
	}
	return *m_Instance;
}