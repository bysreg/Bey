#pragma once

#include "IInputLayout.h"
#include "InputLayoutDesc.h"
#include <vector>

namespace bey {

	class D3DInputLayout : public IInputLayout {
	public:
		//override interface
		virtual void Init(BeyNativeInputLayout* nativeInputLayout, const InputLayoutDesc* inputLayoutDescs, int descNumElements);
		virtual void Clean();

		//*****functions to translate bey::IInputLayout to corresponding value on direct3d****
		

		static LPCSTR ConvertEinputLayoutType(E_INPUT_LAYOUT_TYPE type);
		
		//get direct3d format suitable for each input layout type
		static DXGI_FORMAT GetSuitableFormatFromType(E_INPUT_LAYOUT_TYPE type);		
		//*************************************************************************************

		D3DInputLayout();
		~D3DInputLayout();
	private:
		BeyNativeInputLayout* m_NativeInputLayout;
		std::vector<InputLayoutDesc> m_InputLayoutDescs;		
	};

}