#pragma once

#include "Common\DataTypeDefs.h"
#include <d3d11.h>

namespace bey {

	enum E_BUFFER_USAGE {
		IMMUTABLE = 0,
	};

	enum E_BUFFER_TYPE {
		VERTEX_BUFFER = 0, 
	};

	struct BufferDesc {
		E_BUFFER_USAGE usage;
		E_BUFFER_TYPE type;
		const void* data;
		BeyUint byteSize;

#ifdef DIRECT3D
		static D3D11_USAGE ConvertEBufferUsage(E_BUFFER_USAGE usage);
		static D3D11_BIND_FLAG ConvertEBufferType(E_BUFFER_TYPE type);
#endif
	};
}