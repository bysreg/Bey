#pragma once

#include "Common\DataTypeDefs.h"
#include <d3d11.h>

namespace bey {

	// BU stands for buffer usage
	enum E_BUFFER_USAGE {
		E_BU_IMMUTABLE = 0, // equivalent to direct 3d D3D11_USAGE_IMMUTABLE. GPU can only read. CPU cannot read nor write		
	};

	enum E_BUFFER_TYPE {
		E_BT_VERTEX_BUFFER = 0, // BT stands for buffer type
		E_BT_INDEX_BUFFER,
		E_BT_CONSTANT_BUFFER
	};

	//simple data-only variables
	struct BufferDesc {
		// mandatory data
		E_BUFFER_USAGE usage;
		E_BUFFER_TYPE type;
		const void* data;
		BeyUint totalByteSize;
		BeyUint elementByteSize; // size of an element in the buffer in bytes

#ifdef DIRECT3D
		//convert types to D3D11 corresponding types
		static D3D11_USAGE ConvertEBufferUsage(E_BUFFER_USAGE usage);
		static D3D11_BIND_FLAG ConvertEBufferType(E_BUFFER_TYPE type);
#else
//TODO : open gl here
#endif
	};
}