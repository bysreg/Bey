#pragma once

#include "IShader.h"

//forward declarations
struct ID3D10Blob;

namespace bey {
	class D3DShader : public IShader
	{
	public:
		//override interface
		virtual void Init(const ShaderInitData& data);
		virtual void Clean();		

		D3DShader();
		~D3DShader();		

	private:			
		ShaderInitData* m_Data;
		ID3D10Blob* m_Blob;
	};
}
