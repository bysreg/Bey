#pragma once

#include "IInputLayout.h"
#include "InputLayoutDesc.h"
#include <vector>

namespace bey
{

	class OGLInputLayout : public IInputLayout
	{
	public :
		//override interface
		virtual void Init(BeyNativeInputLayout* nativeInputLayout, const InputLayoutDesc* inputLayoutDesc, int numInputLayoutDescNum);
		virtual void Clean();

		OGLInputLayout();
		~OGLInputLayout();
	private:
		BeyNativeInputLayout* m_NativeInputLayout;
		BeyUint m_NumInputLayout;
		std::vector<InputLayoutDesc> m_InputLayoutDescs;
	};

}