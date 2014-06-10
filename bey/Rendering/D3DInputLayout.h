#pragma once

#include "IInputLayout.h"

namespace bey {

	class D3DInputLayout : public IInputLayout {
	public:
		//override interface
		virtual void Init(BeyNativeInputLayout* nativeInputLayout, const InputLayoutDesc& inputLayoutDesc);
		virtual void Clean();

	};

}