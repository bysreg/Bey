#pragma once

#ifdef WIN32
#include <DirectXMath.h>
#endif

#ifdef WIN32 
typedef float BeyFloat;
typedef int BeyInt;
typedef unsigned int BeyUint;
typedef DirectX::XMFLOAT3 BeyFloat3;
typedef DirectX::XMFLOAT4 BeyFloat4;

#elif

//TODO : need data types to substitute BeyFloat3
//TODO : need data types to substitute BeyFloat4
//TODO : need data types to substitute BeyMatrix

#endif

//Color type. wrapped in namespace
namespace bey {
	typedef BeyFloat4 RGBA_NORM;
}