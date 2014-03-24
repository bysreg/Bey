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

	class D3DRendering : public IRendering {		
	public:		

		IDXGISwapChain* m_SwapChain;
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;           
		ID3D11Texture2D* m_DepthStencilBuffer;
		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11RenderTargetView* m_BackBuffer;
		
		static IRendering* m_Instance;

		//override interface
		virtual void Init(const RenderingInitData* data);
		virtual void Clean();
		virtual void Render();

		static IRendering& GetInstance();
	private:		
		UINT m_4xMsaaQuality;
		bool m_Enable4xMsaa;

		D3DRendering();
		~D3DRendering();
	};

#ifdef WIN32
	typedef D3DRendering Rendering;	
#endif
}