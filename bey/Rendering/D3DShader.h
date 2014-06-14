#pragma once

#include "IShader.h"

namespace bey {
	class D3DShader : public IShader
	{
	public:
		//override interface
		virtual void Init(const ShaderInitData& data);
		virtual void Clean();

		ID3DBlob* GetCompiledShader();

		D3DShader();
		~D3DShader();		

	private:			
		ShaderInitData* m_Data;
		ID3DBlob* m_Blob;
	};
}
