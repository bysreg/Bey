#include "Buffer.h"
#include <cassert>
#include <d3d11.h>

using namespace bey;

Buffer::Buffer(BeyNativeBuffer* nativeBuffer, const BufferDesc& bufferDesc) :
m_NativeBuffer(nativeBuffer), 
bufferDesc(bufferDesc)
{
}

BeyNativeBuffer* Buffer::GetNativeBuffer() const 
{
	return m_NativeBuffer;
}

Buffer::~Buffer()
{
	if (m_NativeBuffer != nullptr)
		m_NativeBuffer->Release();
}