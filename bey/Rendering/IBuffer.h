#pragma once

#include <d3d11.h>

typedef ID3D11Buffer BeyNativeBuffer;

namespace bey {
	//forward declarations
	struct BufferDesc;

	// just a wrapper for GPU buffer
	class IBuffer {
	public:		
		virtual void Init(BeyNativeBuffer* nativeBuffer, const BufferDesc& bufferDesc) = 0;
		virtual void Clean() = 0;
		virtual BeyNativeBuffer* GetNativeBuffer() const = 0;
		virtual void GetDesc(BufferDesc* bufferDesc) const = 0;
	};

}