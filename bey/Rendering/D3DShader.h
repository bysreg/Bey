#pragma once

#include "IShader.h"

//forward declarations
struct ID3D10Blob;

namespace bey {
	class D3DShader : IShader
	{
	public:
		//override interface
		virtual void Init(const ShaderInitData* data);
		virtual void Clean();
		virtual void Compile();

		D3DShader();
		~D3DShader();		

	private:			
		ShaderInitData* m_Data;
		ID3D10Blob* m_Blob;
	};
}
