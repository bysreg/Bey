#pragma once

#include "Log.h"

#ifdef _DEBUG
#include  <CRTDBG.h>
#endif

#if defined _DEBUG
	#if defined _MSC_VER // add more platforms that has support for debugging functions here .. 
		#define BEY_COMMON_DEBUGGING
	#endif
#endif

#ifdef BEY_COMMON_DEBUGGING	
	#define BEY_DEBUG_BREAK(message)\
		do {\
			BEY_LOG(message);\
			_CrtDbgBreak();\
		} while(0)	
#else

	#define BEY_DEBUG_BREAK

#endif