#include "Log.h"
#include <cstdarg>
#include <cstdio>

using namespace bey;

#pragma warning(disable: 4996) /* Disable vsprintf deprecation by MS Compiler */
void Log(const char* filename, int line, const char* format, ...) {
	va_list params;
	char buf[2048];

	va_start(params, format);
	vsprintf(buf, format, params);
	printf("%s", buf);	
	va_end(params);
}
#pragma warning(default: 4996) /* Restore default */