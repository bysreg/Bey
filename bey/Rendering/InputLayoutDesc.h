#pragma once

#include "Common\DataTypeDefs.h"

namespace bey {

	enum E_INPUT_LAYOUT_TYPE {
		E_IL_POSITION = 0, 
		E_IL_COLOR = 1,
	};

	struct InputLayoutDesc {
		E_INPUT_LAYOUT_TYPE type;
		BeyUint index;
		BeyUint stride;
	};

}