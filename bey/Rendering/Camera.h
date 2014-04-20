#pragma once

#include "Common\DataTypeDefs.h"
#include "Common\Matrix4f.h"

namespace bey {

	class Camera {
	public:
		Camera();

	private:
		BeyFloat3 m_Pos;
		BeyFloat3 m_Rot;
		
		Matrix4f m_World;
		Matrix4f m_View;

		void UpdateWorldMatrix();		
		void UpdateViewMatrix();		
	};

}