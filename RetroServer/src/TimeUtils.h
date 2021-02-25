#pragma once
#include <string>

class TimeUtils {
public:
	static time_t GetTimeOfDay();
	static long long GetTimeOfDayMs();
	static std::string GetTimeStr(time_t time);
	static std::string GetCurTimeStr();
	static std::string GetTimeStrWithFormat(const char* format, time_t time);
};