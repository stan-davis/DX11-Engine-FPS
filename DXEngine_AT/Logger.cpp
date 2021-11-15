#include "Logger.h"

void Logger::Log(const char* fmt, ...)
{
	char buffer[1024];
	
	va_list args;
	va_start(args, buffer);
	vsprintf_s(buffer, fmt, args);

	OutputDebugStringA(buffer);
	va_end(args);

	OutputDebugStringA(buffer);
}
