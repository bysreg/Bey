#pragma once

#include "BufferDesc.h"

//forward declarations
struct ID3D11Buffer;

#ifdef DIRECT3D
typedef ID3D11Buffer BeyNativeBuffer; // d3d native buffer
#else
	
#endif

namespace bey {

	// just a wrapper for GPU buffer
	class Buffer {
	public:
		// create a wrapper for GPU buffer. bufferDesc must be the same bufferDesc that is used to create the nativeBuffer
		Buffer(BeyNativeBuffer* nativeBuffer, const BufferDesc& bufferDesc);
		virtual ~Buffer();

		const BufferDesc bufferDesc;

		BeyNativeBuffer* GetNativeBuffer() const;

	private:
		BeyNativeBuffer* m_NativeBuffer;
	};

}