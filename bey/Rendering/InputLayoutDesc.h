#pragma once

#include "Common\DataTypeDefs.h"

namespace bey {

	enum E_INPUT_LAYOUT_TYPE {
		E_IL_POSITION = 0, // float3
		E_IL_COLOR, // float4
	};

	//description of each vertex's data
	//for now, supports only position and color
	struct InputLayoutDesc {
		E_INPUT_LAYOUT_TYPE type; // semantic of that input. what will it be used for
		BeyUint index;
		BeyUint stride;
	};

}