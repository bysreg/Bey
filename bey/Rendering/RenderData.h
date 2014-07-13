#pragma once

#include "Common\DataTypeDefs.h"

namespace bey {

	//forward declarations
	class IBuffer;
	class IShader;
	class IInputLayout;

	enum E_RENDER_TYPE {
		E_USE_INDEX_BUFFER = 0,
	};

	enum E_RENDER_PRIMITIVE_TOPOLOGY {
		E_TRIANGLE_LIST = 0,
	};

	//data to determine the suitable draw operation
	struct RenderData {
		E_RENDER_TYPE renderType;
		IBuffer* vertexBuffer;
		E_RENDER_PRIMITIVE_TOPOLOGY primitiveTopology;
		IShader* vs; // vertex shader
		IShader* fs; // fragment shader
		IInputLayout* inputLayout;

		BeyUint indexCount; // mandatory if using index buffer
		IBuffer* indexBuffer; // mandatory if using index buffer
	};

}