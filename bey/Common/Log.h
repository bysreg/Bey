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

// the following log macro has runtime check in it. why ? so that the compiler will always check that 
// the logging code is valid. Optimizer will remove it anyway when BEY_LOG_TEST is 0.
// the do-while code ensure that this is a full statement so that it could be safely used in if else without curly braces.
// more info here : http://stackoverflow.com/questions/1644868/c-define-macro-for-debug-printing/1644898#1644898
#define BEY_LOG(message)\
	do { if (BEY_LOG_TEST) std::cerr << message << " (log invoked from : " << __FILE__ << ":" << __LINE__ <<  ")" << std::endl; } while (0)

#define BEY_LOGF(format, ...)\
	do { if (BEY_LOG_TEST) bey::Log(__FILE__, __LINE__, format, __VA_ARGS__); } while (0)

