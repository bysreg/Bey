#pragma once

#include <d3d11.h>

#ifdef BEY_USE_DIRECT3D
typedef ID3D11InputLayout BeyNativeInputLayout;
#else
// TODO : open gl here
#endif

namespace bey {

	//forward declarations
	struct InputLayoutDesc;

	class IInputLayout {
	public :
		// override interface
		virtual void Init(BeyNativeInputLayout* nativeInputLayout, const InputLayoutDesc* inputLayoutDesc, int numInputLayoutDescNum) = 0;
		virtual void Clean() = 0;		

		//ctor, dtor
		IInputLayout() {};
		virtual ~IInputLayout() {};
	};
}