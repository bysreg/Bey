#include "D3DInputLayout.h"
#include "Common\Debugging.h"

using namespace bey;

void D3DInputLayout::Init(BeyNativeInputLayout* nativeInputLayout, const InputLayoutDesc* inputLayoutDescs, int descNumElements)
{
	// TODO : checks whether the m_nativeInputLayout is not null. to make sure we dont have a second initialization.
	m_NativeInputLayout = nativeInputLayout;

	//create our own copy of inputlayoutdescs
	for (int i = 0; i < descNumElements; i++) {
		m_InputLayoutDescs.push_back(inputLayoutDescs[i]);
	}	
}

void D3DInputLayout::Clean()
{
	m_NativeInputLayout->Release();
}

LPCSTR D3DInputLayout::ConvertEinputLayoutType(E_INPUT_LAYOUT_TYPE type)
{
	// node defaul in switch is intentional. so that it will create runtime error whenever there's a type with no matching case
	switch (type) {
	case E_IL_COLOR :
		return "COLOR";
	case E_IL_POSITION : 	
		return "POSITION";
	}
	
	BEY_DEBUG_BREAK("no suitable conversion");
	return ""; // error
}

DXGI_FORMAT D3DInputLayout::GetSuitableFormatFromType(E_INPUT_LAYOUT_TYPE type)
{
	switch (type) {
	case E_IL_POSITION :
		return DXGI_FORMAT_R32G32B32_FLOAT;
	case E_IL_COLOR :
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	}

	BEY_DEBUG_BREAK("no suitable conversion");
	return DXGI_FORMAT_R32G32B32_FLOAT; // error, arbitrary return value
}

BeyNativeInputLayout* D3DInputLayout::GetNativeInputLayout()
{
	return m_NativeInputLayout;
}

D3DInputLayout::D3DInputLayout() :
m_NativeInputLayout(nullptr)
{
}

D3DInputLayout::~D3DInputLayout()
{	
}