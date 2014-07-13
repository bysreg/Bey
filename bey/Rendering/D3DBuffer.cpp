#include "D3DBuffer.h"
#include "BufferDesc.h"
#include <d3d11.h>

using namespace bey;

void D3DBuffer::Init(BeyNativeBuffer* nativeBuffer, const BufferDesc& bufferDesc)
{
	m_NativeBuffer = nativeBuffer;
	m_BufferDesc = bufferDesc;
}

void D3DBuffer::Clean()
{	
	if (m_NativeBuffer)
		m_NativeBuffer->Release();
}

BeyNativeBuffer* D3DBuffer::GetNativeBuffer() const
{
	return m_NativeBuffer;
}

void D3DBuffer::GetDesc(BufferDesc* bufferDesc) const
{
	*bufferDesc = m_BufferDesc;
}

D3DBuffer::D3DBuffer() : 
m_NativeBuffer(nullptr)
{
}