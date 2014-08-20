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
#include <comdef.h>

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
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                   // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
	scd.BufferDesc.Width = data->screenWidth;                   // set the back buffer width
	scd.BufferDesc.Height = data->screenHeight;                 // set the back buffer height
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
	scd.OutputWindow = data->handleWindow;                               // the window to be used
	scd.SampleDesc.Count = 4;                              // how many multisamples
	scd.Windowed = TRUE;                                   // windowed/full-screen mode
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&m_SwapChain,
		&m_Device,
		NULL,
		&m_DeviceContext);


	// get the address of the back buffer
	ID3D11Texture2D *pBackBuffer;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// use the back buffer address to create the render target
	m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_BackBuffer);
	pBackBuffer->Release();

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
	bd.CPUAccessFlags = 0; // for now, CPU does not need to read or write to buffer // TODO : must change this if we support other than E_BU_IMMUTABLE
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

	//set shaders
	m_DeviceContext->VSSetShader(vso, nullptr, 0);
	m_DeviceContext->PSSetShader(fso, nullptr, 0);

	//set input layout
	m_DeviceContext->IASetInputLayout(nativeInputlayout);

	//set vertex buffer
	BindBuffer(*(renderData.vertexBuffer));

	//set primitive topology
	switch (renderData.primitiveTopology) {
	case E_TRIANGLE_LIST:
	default:
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	}

	switch (renderData.renderType) {		
	case E_USE_INDEX_BUFFER:
		BindBuffer(*(renderData.indexBuffer)); // set index buffer
		m_DeviceContext->DrawIndexed(renderData.indexCount, 0, 0);
		break;

	case E_USE_VERTEX_BUFFER_ONLY:
	default:
		m_DeviceContext->Draw(renderData.vertexCount, 0);
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
		{			
			BEY_LOGF("compile error on %s => ", compileShaderData.filepath);
			BEY_LOGW(_com_error(hr).ErrorMessage());
		}
			
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

	HR(m_Device->CreateInputLayout(d3dDescs, numInputLayoutDesc, nativeCompiledShaderPointer, nativeCompiledShaderSize, &nativeInputLayout));

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
			HR(m_Device->CreatePixelShader(sid.nativeProgram->GetBufferPointer(), sid.nativeProgram->GetBufferSize(), nullptr, &pixelShaderObject));
			sid.fragmentShaderObject = pixelShaderObject;
		}		
		break;
	case E_VERTEX_SHADER:
	default:
		{
			ID3D11VertexShader* vertexShaderObject = nullptr;
			HR(m_Device->CreateVertexShader(sid.nativeProgram->GetBufferPointer(), sid.nativeProgram->GetBufferSize(), nullptr, &vertexShaderObject));
			sid.vertexShaderObject = vertexShaderObject;
		}		
		break;		
	}	

	shader->Init(sid);

	return shader;
}