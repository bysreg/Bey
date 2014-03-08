#include "Rendering.h"
#include "IRendering.h"
#include "D3DRendering.h"

using namespace bey;

IRendering* Rendering::m_rendering = NULL;

Rendering::Rendering()
{
}

Rendering::~Rendering()
{
}

IRendering& Rendering::GetInstance() {
	if (m_rendering == NULL) {		
		m_rendering = new D3DRendering;		
	}
	return *m_rendering;
}