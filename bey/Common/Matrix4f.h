#pragma once

#include "DataTypeDefs.h"

namespace bey {
	//forward declarations
	struct Vector3f;

	class Matrix4f {
	public:
		//constructors
		Matrix4f() {}
		Matrix4f(BeyFloat val);
		Matrix4f(const Matrix4f & mat);

		// Matrix4f operations
		Matrix4f & SetZero();
		Matrix4f & SetIdentity();

		Matrix4f & SetRotationX(BeyFloat angle);
		Matrix4f & SetRotationY(BeyFloat angle);
		Matrix4f & SetRotationZ(BeyFloat angle);
		Matrix4f & SetRotationAngleAxis(float angle, float x, float y, float z);

		Matrix4f & SetScale(BeyFloat scale);
		Matrix4f & SetScale(BeyFloat scaleX, BeyFloat scaleY, BeyFloat scaleZ);
		Matrix4f & SetScale(const BeyFloat * pScale);
		Matrix4f & SetScale(const Vector3f * scaleVec);

		Matrix4f & SetTranslation(BeyFloat x, BeyFloat y, BeyFloat z);
		Matrix4f & SetTranslation(const BeyFloat *pTrans);
		Matrix4f & SetTranslation(const Vector3f * vec);

		Matrix4f & SetPerspective(BeyFloat fovY, BeyFloat aspect, BeyFloat nearPlane, BeyFloat farPlane);

		Matrix4f Transpose();

		Matrix4f operator + (const Matrix4f & mat);
		Matrix4f & operator += (const Matrix4f & mat);
		Matrix4f operator - (const Matrix4f & mat);
		Matrix4f &operator -= (const Matrix4f & mat);

		Matrix4f operator * (const Matrix4f & mat);
		Matrix4f operator * (BeyFloat k);
		Matrix4f & operator *= (BeyFloat k);		

		Matrix4f & operator = (const Matrix4f & mat);

	private:
		BeyFloat m[4][4];
	};
}