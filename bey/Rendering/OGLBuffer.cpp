#include "OGLBuffer.h"
#include "BufferDesc.h"

using namespace bey;

void OGLBuffer::Init(BeyNativeBuffer* nativeBuffer, const BufferDesc& bufferDesc)
{
	m_NativeBuffer = nativeBuffer;
	m_BufferDesc = bufferDesc;
}

void OGLBuffer::Clean()
{
	if (m_NativeBuffer)
		delete m_NativeBuffer;
}

BeyNativeBuffer* OGLBuffer::GetNativeBuffer() const
{
	return m_NativeBuffer;
}

void OGLBuffer::GetDesc(BufferDesc* bufferDesc) const
{
	*bufferDesc = m_BufferDesc;
}

OGLBuffer::OGLBuffer() :
m_NativeBuffer(nullptr)
{
}