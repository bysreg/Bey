#include "BufferDesc.h"
#include <cassert>

using namespace bey;

#ifdef DIRECT3D

D3D11_USAGE BufferDesc::ConvertEBufferUsage(E_BUFFER_USAGE usage) {
	switch (usage) {
	case E_BU_IMMUTABLE : 
		return D3D11_USAGE_IMMUTABLE;
	}

	assert(false); // force quit
}

D3D11_BIND_FLAG BufferDesc::ConvertEBufferType(E_BUFFER_TYPE type) {
	switch (type){
	case E_BT_VERTEX_BUFFER:
		return D3D11_BIND_VERTEX_BUFFER;
	case E_BT_INDEX_BUFFER:
		return D3D11_BIND_INDEX_BUFFER;
	}

	assert(false); // force quit
}

#endif // -- DIRECT3D