#include "Log.h"
#include <cstdarg>

using namespace bey;

void Log(const char* filename, int line, const char* format, ...) {
	va_list params;
	char buf[2048];

	va_start(params, format);
	vsprintf_s(buf, sizeof(buf), format, params);

	printf("%s", buf);
	OutputDebugString(buf);

	va_end(params);
}