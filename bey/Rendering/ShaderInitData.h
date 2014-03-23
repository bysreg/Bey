#pragma once

#include <windows.h>
#include <string>

namespace bey {
	enum ShaderType {
		Vertex = 0,
		Fragment
	};

	struct ShaderInitData {
		std::string filepath;
		ShaderType type;
#ifdef WIN32
		LPCSTR shaderProfile;
#endif
	};
}