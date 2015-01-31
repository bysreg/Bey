#pragma once

#include "IBuffer.h"
#include "BufferDesc.h"

namespace bey
{
	class OGLBuffer : public IBuffer
	{
	public:
		//override interface
		virtual void Init(BeyNativeBuffer* nativeBuffer, const BufferDesc& bufferDesc);
		virtual void Clean();
		virtual BeyNativeBuffer* GetNativeBuffer() const;
		virtual void GetDesc(BufferDesc* bufferDesc) const;

		OGLBuffer();
	private:
		GLuint* m_NativeBuffer;
		BufferDesc m_BufferDesc;
	};
}