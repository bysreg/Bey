#include "Buffer.h"
#include <cassert>
#include <d3d11.h>

using namespace bey;

Buffer::Buffer() :
#ifdef DIRECT3D
m_NativeBuffer(nullptr)
#endif
{
}

Buffer::~Buffer()
{
	if (m_NativeBuffer != nullptr)
		m_NativeBuffer->Release();
}

void Buffer::SetNativeBuffer(ID3D11Buffer* nativeBuffer)
{
	assert(m_NativeBuffer == nullptr); // make sure no native buffer already

	m_NativeBuffer = nativeBuffer;
}