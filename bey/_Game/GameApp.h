#pragma once

#include "Framework\App.h"
#include "windows.h"

namespace bey {
	//forward declarations
	class IRendering;

	class GameApp : public App {
	public:
		//override
		virtual void Init();
		virtual void Clean();
		virtual void Update(float dt);
		virtual void Render();

		//overload
		virtual void Init(int width, int height, HWND hWnd);

		GameApp();		
	private:
		int m_Width;
		int m_Height;
		HWND m_Hwnd;
		IRendering* m_Rendering;
	};
}