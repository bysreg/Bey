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
		ID3D11VertexShader* GetVertexShaderObject();
		ID3D11PixelShader* GetFragmentShaderObject();

		D3DShader();
		~D3DShader();		

	private:			
		ShaderInitData* m_Data;
		ID3DBlob* m_Blob;
		ID3D11PixelShader* m_FragmenShaderObject;
		ID3D11VertexShader* m_VertexShaderObject;
	};
}
