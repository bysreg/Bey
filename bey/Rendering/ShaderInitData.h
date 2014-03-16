#pragma once

#include <windows.h>

namespace bey {
	enum ShaderType {
		Vertex = 0,
		Fragment
	};

	struct ShaderInitData {
		char* filepath;
		ShaderType type;
#ifdef WIN32
		LPCSTR shaderProfile;
#endif

		ShaderInitData();		
	};
}