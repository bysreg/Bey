#pragma once

namespace bey {

	//forward declarations for namespace bey
	struct ShaderInitData;

	class IShader {		
		virtual void Init(const ShaderInitData* data) = 0;
		virtual void Clean() = 0;

		virtual void Compile() = 0;				
	};

}