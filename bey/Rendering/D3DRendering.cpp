#include "D3DRendering.h"
#include "RenderingInitData.h"
#include "BufferDesc.h"
#include "D3DUtil.h"
#include "D3DBuffer.h"
#include "RenderData.h"
#include "D3DShader.h"
#include "Common\Log.h"
#include "InputLayoutDesc.h"
#include "D3DInputLayout.h"
#include <cstring>
#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <assert.h>
#include <cstdio>

using namespace bey;

D3DRendering::D3DRendering() : 
m_SwapChain(nullptr), 
m_Device(nullptr),
m_DeviceContext(nullptr),
m_BackBuffer(nullptr)
{
}

D3DRendering::~D3DRendering()
{			
}

//initialize for rendering component
//assume d3d11 compatible
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
	ReleaseCOM(m_DepthStencilView);	
	ReleaseCOM(m_DepthStencilBuffer);
	ReleaseCOM(m_BackBuffer);
	ReleaseCOM(m_SwapChain);
	ReleaseCOM(m_DeviceContext);

#if defined _DEBUG || defined DEBUG
	ID3D11Debug* d3dDebug = nullptr;
	m_Device->QueryInterface(__uuidof(ID3D11Debug), (void**) &d3dDebug);	
	d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_SUMMARY | D3D11_RLDO_DETAIL);
	ReleaseCOM(d3dDebug);
#endif

	ReleaseCOM(m_Device); // last D3D COM to be released
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

IBuffer* D3DRendering::CreateBuffer(const BufferDesc* bufferDesc)
{
	D3D11_BUFFER_DESC bd;	
	bd.Usage = BufferDesc::ConvertEBufferUsage(bufferDesc->usage);
	bd.ByteWidth = bufferDesc->totalByteSize;
	bd.BindFlags = BufferDesc::ConvertEBufferType(bufferDesc->type);
	bd.CPUAccessFlags = 0; // for now, CPU does not need to read or write to buffer
	bd.MiscFlags = 0; // no misc flags for now
	bd.StructureByteStride = 0; // always 0 for now

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = bufferDesc->data;

	ID3D11Buffer* nativeBuffer;
	HR(GetDevice()->CreateBuffer(&bd, &initData, &nativeBuffer));

	//store it in our wrapper for native buffer
	IBuffer* buffer = new D3DBuffer;		
	buffer->Init(nativeBuffer, *bufferDesc);

	return buffer;	
}

void D3DRendering::BindBuffer(const IBuffer& buffer) 
{
	ID3D11Buffer* nativeBuffer = buffer.GetNativeBuffer();
	BufferDesc bufferDesc;
	buffer.GetDesc(&bufferDesc);
	switch (bufferDesc.type) {
	case E_BT_VERTEX_BUFFER:
		{
			UINT stride = bufferDesc.elementByteSize;
			UINT offset = 0;

			m_DeviceContext->IASetVertexBuffers(0, 1, &nativeBuffer, &stride, &offset);
			break;
		}
	case E_BT_INDEX_BUFFER:
	default:
		{
			m_DeviceContext->IASetIndexBuffer(nativeBuffer, DXGI_FORMAT_R32_UINT, 0); // IASetIndexBuffer only allows format DXGI_FORMAT_R_16_UINT or DXGI_FORMAT_R32_UINT
			break;
		}
	}
}

void D3DRendering::Render(const RenderData& renderData)
{
	// TODO : because there are static_casts in here, make sure all required datas in renderData is not null and proper

	ID3D11VertexShader* vso = static_cast<D3DShader*>(renderData.vs)->GetVertexShaderObject();
	ID3D11PixelShader* fso = static_cast<D3DShader*>(renderData.fs)->GetFragmentShaderObject();
	ID3D11InputLayout* nativeInputlayout = static_cast<D3DInputLayout*>(renderData.inputLayout)->GetNativeInputLayout();

	//set vertex buffer
	BindBuffer(*(renderData.vertexBuffer));

	//set shaders
	m_DeviceContext->VSSetShader(vso, nullptr, 0);
	m_DeviceContext->PSSetShader(fso, nullptr, 0);

	//set input layout
	m_DeviceContext->IASetInputLayout(nativeInputlayout);

	//set primitive topology
	switch (renderData.primitiveTopology) {
	case E_TRIANGLE_LIST:
	default:
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	}

	switch (renderData.renderType) {		
	case E_USE_INDEX_BUFFER:
	default:
		// TODO : temporarily default to use index buffer. should be to vertex buffer only though
		BindBuffer(*(renderData.indexBuffer)); // set index buffer
		m_DeviceContext->DrawIndexed(renderData.indexCount, 0, 0);
		break;
	}
}

IShader* D3DRendering::CompileShader(const CompileShaderData& compileShaderData)
{	
	ID3DBlob* blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS; // forbids using deprecated syntax
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3DCOMPILE_DEBUG;
#endif

	//convert filename to wide string
	int filenameLength = strlen(compileShaderData.filepath);
	wchar_t* tempFilename = new wchar_t[filenameLength + 1]; // temporary buffer to store the filename in LPCWSTR
	int ret = mbstowcs(tempFilename, compileShaderData.filepath, filenameLength + 1);

	//decides entrypoint and shader profile
	LPCSTR entryPoint = nullptr;
	LPCSTR profile = nullptr;
	switch (compileShaderData.shaderType) {
	case E_FRAGMENT_SHADER:
		entryPoint = "FS";
		profile = "ps_5_0"; // pixel shader
		break;
	case E_VERTEX_SHADER:
	default:
		entryPoint = "VS";
		profile = "vs_5_0";
		break;
	}	

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr; // contains compile error if any, or null if success
	//TODO : change to just use D3DCompile. which means that we use our own file read of the shader source file
	HRESULT hr = D3DCompileFromFile(tempFilename, 
						nullptr, 
						D3D_COMPILE_STANDARD_FILE_INCLUDE,
						entryPoint, 
						profile,
						flags, 
						0, 
						&shaderBlob, 
						&errorBlob);

	delete[] tempFilename;
	
	if (FAILED(hr))
	{
		char* errorMessage = nullptr;
		if (errorBlob != nullptr)
			errorMessage = (char*)errorBlob->GetBufferPointer();
		if (errorMessage != nullptr)
			BEY_LOGF("compile error on %s => %s\n", compileShaderData.filepath, errorMessage);
		else
			BEY_LOGF("compile error on %s\n", compileShaderData.filepath);
		assert(false); // force quit, the message may show up on console output or log
		return nullptr; // never reach this
	}

	//create shader object out of the native compiled shader (because of course you can compile the shaders not in runtime)
	return CreateShader(shaderBlob, compileShaderData.shaderType);
}

IInputLayout* D3DRendering::CreateInputLayout(const InputLayoutDesc* inputLayoutDesc, int numInputLayoutDesc, IShader* compiledShader)
{
	D3D11_INPUT_ELEMENT_DESC* d3dDescs = new D3D11_INPUT_ELEMENT_DESC[numInputLayoutDesc];
	ID3D11InputLayout* nativeInputLayout = nullptr;
	D3DInputLayout* inputLayout = new D3DInputLayout;

	for (int i = 0; i < numInputLayoutDesc; i++) {
		d3dDescs[i].SemanticName = D3DInputLayout::ConvertEinputLayoutType(inputLayoutDesc[i].type);
		d3dDescs[i].SemanticIndex = inputLayoutDesc[i].index;
		d3dDescs[i].Format = D3DInputLayout::GetSuitableFormatFromType(inputLayoutDesc[i].type);
		d3dDescs[i].InputSlot = 0;
		d3dDescs[i].AlignedByteOffset = inputLayoutDesc[i].stride;
		d3dDescs[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		d3dDescs[i].InstanceDataStepRate = 0;
	}	

	D3DShader* d3dCompiledShader = static_cast<D3DShader*>(compiledShader);
	LPVOID nativeCompiledShaderPointer = d3dCompiledShader->GetCompiledShader()->GetBufferPointer();
	SIZE_T nativeCompiledShaderSize = d3dCompiledShader->GetCompiledShader()->GetBufferSize();

	HR(m_Device->CreateInputLayout(d3dDescs, numInputLayoutDesc, nativeCompiledShaderPointer, nativeCompiledShaderSize, &nativeInputLayout)); // TODO : use compiledshader or the shader object ?

	inputLayout->Init(nativeInputLayout, inputLayoutDesc, numInputLayoutDesc);

	delete[] d3dDescs;
	return inputLayout;
}

ID3D11Device* D3DRendering::GetDevice()
{
	return m_Device;
}

D3DShader* D3DRendering::CreateShader(ID3DBlob* shaderProgram, E_SHADER_TYPE shaderType)
{
	D3DShader* shader = new D3DShader;

	ShaderInitData sid;
	sid.shaderType = shaderType;
	sid.nativeProgram = shaderProgram;

	//create the vertex/fragment shader object from the compiled shader	
	switch (shaderType) {
	case E_FRAGMENT_SHADER:
		{
			ID3D11PixelShader* pixelShaderObject = nullptr;
			HR(m_Device->CreatePixelShader(shaderProgram->GetBufferPointer(), shaderProgram->GetBufferSize(), nullptr, &pixelShaderObject));
			sid.fragmentShaderObject = pixelShaderObject;
		}		
		break;
	case E_VERTEX_SHADER:
	default:
		{
			ID3D11VertexShader* vertexShaderObject = nullptr;
			HR(m_Device->CreateVertexShader(shaderProgram->GetBufferPointer(), shaderProgram->GetBufferSize(), nullptr, &vertexShaderObject));
			sid.vertexShaderObject = vertexShaderObject;
		}		
		break;		
	}	

	shader->Init(sid);

	return shader;
}