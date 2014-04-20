#include "Vector3f.h"

using namespace bey;

BeyFloat Vector3f::Length()
{
	return sqrt(x*x + y*y + z*z);
}

Vector3f & Vector3f::Normalize()
{
	BeyFloat lenInv = 1.0f / Length();
	x *= lenInv;
	y *= lenInv;
	z *= lenInv;

	return *this;
}

Vector3f Vector3f::operator + (const Vector3f & vector)
{
	return Vector3f(x + vector.x, y + vector.y, z + vector.z);
}

Vector3f & Vector3f::operator += (const Vector3f & vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;

	return *this;
}

Vector3f Vector3f::operator - ()
{
	return Vector3f(-x, -y, -z);
}

Vector3f Vector3f::operator - (const Vector3f & vector)
{
	return Vector3f(x - vector.x, y - vector.y, z - vector.z);
}

Vector3f & Vector3f::operator -= (const Vector3f & vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;

	return *this;
}

Vector3f Vector3f::operator * (BeyFloat k)
{
	return Vector3f(x * k, y * k, z * k);
}

Vector3f & Vector3f::operator *= (BeyFloat k)
{
	x *= k;
	y *= k;
	z *= k;

	return *this;
}

Vector3f Vector3f::operator / (BeyFloat k)
{
	BeyFloat kInv = 1.0f / k;
	return Vector3f(x * kInv, y * kInv, z * kInv);
}

Vector3f & Vector3f::operator /= (BeyFloat k)
{
	return operator *= (1.0f / k);
}

Vector3f & Vector3f::operator = (const Vector3f & vector)
{
	x = vector.x;
	y = vector.y;
	z = vector.z;

	return *this;
}

BeyFloat Vector3f::operator [] (unsigned int idx)
{
	return (&x)[idx];
}

Vector3f Vector3f::Modulate(const Vector3f & vector)
{
	return Vector3f(x * vector.x, y * vector.y, z * vector.z);
}

BeyFloat Vector3f::Dot(const Vector3f & vector)
{
	return x * vector.x + y * vector.y + z * vector.z;
}

Vector3f Vector3f::Cross(const Vector3f & vector)
{
	return Vector3f(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
}