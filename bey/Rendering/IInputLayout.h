#pragma once

#ifdef BEY_USE_DIRECT3D
#include <d3d11.h>
typedef ID3D11InputLayout BeyNativeInputLayout;
#elif BEY_USE_OPENGL
#include <gl\GL.h>
typedef GLuint BeyNativeInputLayout;
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