#ifdef WIN32
#include <DirectXMath.h>
#endif

#ifdef WIN32 

typedef DirectX::XMFLOAT3 BeyFloat3;
typedef DirectX::XMFLOAT4 BeyFloat4;

#elif

//TODO : need data types to substitute BeyFloat3
//TODO : need data types to substitute BeyFloat4

#endif

//Color type. wrapped in namespace
namespace bey {
	typedef DirectX::XMFLOAT4 RGBA_NORM;	
}