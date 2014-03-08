#pragma once

#include "IRendering.h"
#include "d3d11.h"
#include <windows.h>

namespace bey {

	struct D3DRenderingInitData : public RenderingInitData {
		HWND handleWindow;
	};

	class D3DRendering : public IRendering {
	public:
		friend class Rendering;

		IDXGISwapChain *m_SwapChain;             // the pointer to the swap chain interface
		ID3D11Device *m_Device;                     // the pointer to our Direct3D device interface
		ID3D11DeviceContext *m_DeviceContext;           // the pointer to our Direct3D device context
		ID3D11RenderTargetView *m_BackBuffer;    // pointer to backbuffer

		//override interface
		virtual void Init(const RenderingInitData& data);
		virtual void Clean();
		virtual void Render();

	private:		
		D3DRendering();
		~D3DRendering();
	};

}