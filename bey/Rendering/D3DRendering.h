#pragma once

#include "IRendering.h"
#include <windows.h>

//forward declarations
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11DepthStencilView;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;

namespace bey {

	//forward declarations for namespace bey
	class D3DShader;
	enum E_SHADER_TYPE : unsigned int;

	//singleton
	class D3DRendering : public IRendering {		
	public:				
		//override interface
		virtual void Init(const RenderingInitData* data);
		virtual void Clean();
		virtual void Clear();
		virtual void SwapBuffer();	
		virtual IBuffer* CreateBuffer(const BufferDesc* bufferDesc);
		virtual void BindBuffer(const IBuffer& buffer);
		virtual void Render(const RenderData& renderData);
		virtual IShader* CompileShader(const CompileShaderData& compileShaderData);
		virtual IInputLayout* CreateInputLayout(const InputLayoutDesc* inputLayoutDesc, int numInputLayoutDesc, IShader* compiledShader);

		D3DRendering();
		~D3DRendering();

		//specific d3d
		ID3D11Device* GetDevice();

	private:				
		IDXGISwapChain* m_SwapChain;
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;		
		ID3D11RenderTargetView* m_BackBuffer;
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11DepthStencilView* m_DepthStencilView;		

		D3DRendering(D3DRendering const&); // don't implement
		void operator=(D3DRendering const&); // don't implement

		D3DShader* CreateShader(ID3DBlob* shaderProgram, E_SHADER_TYPE shaderType);
	};
}