#pragma once

#include "IBuffer.h"
#include "BufferDesc.h"

namespace bey {
	class D3DBuffer : public IBuffer 
	{
	public :
		//override interface		
		virtual void Init(BeyNativeBuffer* nativeBuffer, const BufferDesc& bufferDesc);
		virtual void Clean();
		virtual BeyNativeBuffer* GetNativeBuffer() const;
		virtual void GetDesc(BufferDesc* bufferDesc) const;

		D3DBuffer();
	private:
		ID3D11Buffer* m_NativeBuffer;
		BufferDesc m_BufferDesc;
	};
}