#pragma once

namespace bey {

	enum E_RENDER_TYPE {
		E_USE_INDEX_BUFFER = 0,
	};

	//data to determine the suitable draw operation
	struct RenderData {
		E_RENDER_TYPE renderType;
		
		// mandatory if using index buffer
		BeyUint indexCount; 
	};

}