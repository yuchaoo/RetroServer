#pragma once
#include <iostream>
#include <assert.h>

void Assert(bool cond, const char* msg);

class LogUtils {
public:
	enum Level {
		kError,
		kWarning,
		kInfo,
		kDebug,
	};
	static bool Open(const char* logFile);
	static void Close();
	static void LogError(const char* format, ...);
	static void LogDebug(const char* format, ...);
	static void LogWarning(const char* format, ...);
	static void LogMsg(const char* format, Level lv, va_list argp);
private:
	static FILE* _file;
};