#include "OGLInputLayout.h"

using namespace bey;

void OGLInputLayout::Init(BeyNativeInputLayout* nativeInputLayout, const InputLayoutDesc* inputLayoutDesc, int numInputLayoutDescNum)
{
	// TODO : checks whether the m_nativeInputLayout is not null. to make sure we dont have a second initialization.
	m_NativeInputLayout = nativeInputLayout;
	m_NumInputLayout = numInputLayoutDescNum;

	//create our own copy of inputlayoutdescs
	for (int i = 0; i < numInputLayoutDescNum; i++) {
		m_InputLayoutDescs.push_back(inputLayoutDesc[i]);
	}
}

void OGLInputLayout::Clean()
{
	delete m_NativeInputLayout;
}

OGLInputLayout::OGLInputLayout() : 
m_NativeInputLayout(nullptr)
{
}

OGLInputLayout::~OGLInputLayout()
{
}