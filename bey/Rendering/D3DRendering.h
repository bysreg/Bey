#pragma once

#include "IRendering.h"

#include <windows.h>

//forward declarations
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

namespace bey {

	class D3DRendering : public IRendering {		
	public:		

		IDXGISwapChain *m_SwapChain;             // the pointer to the swap chain interface
		ID3D11Device *m_Device;                     // the pointer to our Direct3D device interface
		ID3D11DeviceContext *m_DeviceContext;           // the pointer to our Direct3D device context
		ID3D11RenderTargetView *m_BackBuffer;    // pointer to backbuffer
		
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