#pragma once

#include "Common\DataTypeDefs.h"

namespace bey {

	//forward declarations
	class IBuffer;
	class IShader;
	class IInputLayout;

	enum E_RENDER_TYPE {
		E_USE_INDEX_BUFFER = 0,
		E_USE_VERTEX_BUFFER_ONLY,
	};

	enum E_RENDER_PRIMITIVE_TOPOLOGY {
		E_TRIANGLE_LIST = 0, // interpret data as a list of triangles
	};

	//data to determine the suitable draw operation
	struct RenderData {
		E_RENDER_TYPE renderType;
		IBuffer* vertexBuffer;
		BeyUint vertexCount;
		E_RENDER_PRIMITIVE_TOPOLOGY primitiveTopology;
		IShader* vs; // vertex shader
		IShader* fs; // fragment shader
		IInputLayout* inputLayout;

		BeyUint indexCount; // mandatory if using index buffer
		IBuffer* indexBuffer; // mandatory if using index buffer
	};

}