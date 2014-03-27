#pragma once

#include <iostream>

namespace bey {
	void Log(const char* filename, int line, const char* format, ...);
}

#ifdef ENABLE_BEY_LOG
#define BEY_LOG_TEST 1
#else
#define BEY_LOG_TEST 0
#endif

#define BEY_LOG(message)\
	do { if (BEY_LOG_TEST) std::cerr << message << " (log invoked from : " << __FILE__ << ":" << __LINE__ <<  ")" << std::endl; } while (0)

#define BEY_LOGF(format, ...)\
	do { if (BEY_LOG_TEST) bey::Log(__FILE__, __LINE__, format, __VA_ARGS__); } while (0)

