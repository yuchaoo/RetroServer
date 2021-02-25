#include "LogUtils.h"
#include <stdio.h>
#include <stdarg.h>
#include "TimeUtils.h"

FILE* LogUtils::_file = NULL;

void Assert(bool cond, const char* msg) {
	if (!cond) {
		LogUtils::LogError(msg);
	}
	assert(cond);
}

bool LogUtils::Open(const char* logFile) {
	_file = fopen(logFile, "wb");
	if (!_file) {
		LogError("open log file failed, file:%s", logFile);
		return false;
	}
	return true;
}

void LogUtils::Close() {
	if (_file) {
		fclose(_file);
		_file = NULL;
	}
}

void LogUtils::LogError(const char* format, ...) {
	va_list args;
	va_start(args, format);
	LogMsg(format, Level::kError, args);
	va_end(args);
}

void LogUtils::LogDebug(const char* format, ...) {
	va_list args;
	va_start(args, format);
	LogMsg(format, Level::kDebug, args);
	va_end(args);
}

void LogUtils::LogWarning(const char* format, ...) {
	va_list args;
	va_start(args, format);
	LogMsg(format, Level::kWarning, args);
	va_end(args);
}

void LogUtils::LogMsg(const char* format, Level lv, va_list args) {
	static char buf[1024];

	vsnprintf(buf, sizeof(buf), format, args);
	std::string str = TimeUtils::GetCurTimeStr() + " " + buf + "\n";
	if (lv == Level::kError) {
		printf("\x1b[%d;%dm%s\x1b[0m", 40, 41, str.c_str());
	}
	else if (lv == Level::kWarning) {
		printf("\x1b[%d;%dm%s\x1b[0m", 40, 43, str.c_str());
	}
	else {
		printf(str.c_str());
	}

	if (lv <= Level::kInfo && _file) {
		fwrite(str.c_str(), 1, str.size(), _file);
	}
}