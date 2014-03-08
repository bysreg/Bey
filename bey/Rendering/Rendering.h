#pragma once

#include "IRendering.h"

namespace bey {
	
	class Rendering
	{
	public:				
		static IRendering* m_rendering;

		static IRendering& GetInstance();
	private:				
		int type;

		Rendering();
		~Rendering();
	};

}