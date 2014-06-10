#pragma once

#include <d3d11.h>

#ifdef DIRECT3D
typedef ID3D11InputLayout BeyNativeInputLayout;
#else
// TODO
#endif

namespace bey {

	//forward declarations
	struct InputLayoutDesc;

	class IInputLayout {
	public :
		// override interface
		virtual void Init(BeyNativeInputLayout* nativeInputLayout, const InputLayoutDesc& inputLayoutDesc) = 0;
		virtual void Clean() = 0;		

		// TODO : virtual dtor ?
	};
}