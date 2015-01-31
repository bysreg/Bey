#pragma once

#ifdef BEY_USE_DIRECT3D
#include <d3d11.h>
typedef ID3D11Buffer BeyNativeBuffer;
#elif BEY_USE_OPENGL
#include <glew.h>
typedef GLuint BeyNativeBuffer;
#endif

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

		//ctor, dtor
		IBuffer() {};
		virtual ~IBuffer() {};
	};

}