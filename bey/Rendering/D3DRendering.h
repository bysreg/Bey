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

namespace bey {

	//singleton
	class D3DRendering : public IRendering {		
	public:				
		//override interface
		virtual void Init(const RenderingInitData* data);
		virtual void Clean();
		virtual void Clear();
		virtual void SwapBuffer();	
		virtual Buffer* CreateBuffer(const BufferDesc* bufferDesc);
		virtual void BindBuffer(const Buffer& buffer, int slot);
		virtual void Render(const RenderData& renderData);

		D3DRendering();
		~D3DRendering();

		//specific d3d
		ID3D11Device* GetDevice();

	private:				
		IDXGISwapChain* m_SwapChain;
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11RenderTargetView* m_BackBuffer;
		UINT m_4xMsaaQuality;
		bool m_Enable4xMsaa;		

		D3DRendering(D3DRendering const&); // don't implement
		void operator=(D3DRendering const&); // don't implement
	};
}