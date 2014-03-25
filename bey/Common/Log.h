#pragma once

#ifdef ENABLE_BEY_LOG
#define BEY_LOG_TEST 1
#else
#define BEY_LOG_TEST 0
#endif

#define BEY_LOG(message)\
	do { if (BEY_LOG_TEST) std::cerr << message << " (" << __FILE__ << ":" << __LINE__ <<  ")" << std::endl; } while (0)

namespace bey {
	void Log(const char* filename, int line, const char* format, ...);
}