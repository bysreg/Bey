#pragma once

#include "Framework\App.h"
#include "windows.h"
#include "Rendering\RenderData.h"

namespace bey {
	//forward declarations
	class IRendering;
	class IBuffer;
	class IInputLayout;
	class IShader;

	class GameApp : public App {
	public:
		//override
		virtual void Init();
		virtual void Clean();
		virtual void Update(float dt);
		virtual void Render();

		//overload
		virtual void Init(int width, int height, HWND hWnd);

		void CalculateFps(float dt);

		GameApp();
	private:
		int m_Width;
		int m_Height;
		HWND m_Hwnd;
		IRendering* m_Rendering;

		//cube
		IBuffer* m_IndexBuffer;
		IBuffer* m_VertexBuffer;
		IInputLayout* m_InputLayout;
		IShader* m_Vs;
		IShader* m_Fs;
		RenderData m_RenderData;

		void CompileShaders();
		void CreateVertexLayout();
		void CreateConstantBuffer();
	};
}