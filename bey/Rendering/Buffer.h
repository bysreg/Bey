#pragma once

//forward declarations
struct ID3D11Buffer;

namespace bey {

	//wrapper for GPU buffer
	class Buffer {
	public:
		Buffer();		
		virtual ~Buffer();

#ifdef DIRECT3D
		void SetNativeBuffer(ID3D11Buffer* nativeBuffer);
#endif

	private:
#ifdef DIRECT3D
		ID3D11Buffer* m_NativeBuffer; // d3d native buffer
#endif
	};

}