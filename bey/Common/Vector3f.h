#pragma once

#include "DataTypeDefs.h"

namespace bey {
	struct Vector3f {
		//Constructors
		Vector3f() : x(0.0f), y(0.0f), z(0.0f) {}
		Vector3f(BeyFloat _x, BeyFloat _y, BeyFloat _z) : x(_x), y(_y), z(_z) {}
		Vector3f(BeyFloat * pArg) : x(pArg[0]), y(pArg[1]), z(pArg[2]) {}
		Vector3f(const Vector3f & vector) : x(vector.x), y(vector.y), z(vector.z) {}

		//Vector's operations
		BeyFloat Length();
		Vector3f & Normalize();
		Vector3f operator + (const Vector3f & vector);
		Vector3f & operator += (const Vector3f & vector);
		Vector3f operator - ();
		Vector3f operator - (const Vector3f & vector);
		Vector3f & operator -= (const Vector3f & vector);
		Vector3f operator * (BeyFloat k);
		Vector3f & operator *= (BeyFloat k);
		Vector3f operator / (BeyFloat k);
		Vector3f & operator /= (BeyFloat k);
		Vector3f & operator = (const Vector3f & vector);
		Vector3f Modulate(const Vector3f & vector);
		BeyFloat Dot(const Vector3f & vector);
		Vector3f Cross(const Vector3f & vector);

		//access to elements
		BeyFloat operator [] (unsigned int idx);

		// data members
		BeyFloat x;
		BeyFloat y;
		BeyFloat z;
	};
}