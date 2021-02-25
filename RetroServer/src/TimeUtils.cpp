#include "TimeUtils.h"
#include "event2/util.h"

time_t TimeUtils::GetTimeOfDay() {
	struct timeval tv;
	evutil_gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1000000;
}

long long TimeUtils::GetTimeOfDayMs() {
	struct timeval tv;
	evutil_gettimeofday(&tv, NULL);
	long long ret = tv.tv_sec;
	ret = ret * 1000 + tv.tv_usec / 1000;
	return ret;
}

std::string TimeUtils::GetTimeStr(time_t time) {
	static char s[256];
	struct tm* tm = localtime(&time);
	snprintf(s, sizeof(s), "%d-%d-%d %d:%d:%d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
	return s;
}

std::string TimeUtils::GetCurTimeStr() {
	return GetTimeStr(GetTimeOfDay());
}

std::string TimeUtils::GetTimeStrWithFormat(const char* format, time_t time) {
	static char tmp[32];
	size_t pos = 0;

	std::string s = format;
	struct tm* tm = localtime(&time);

	const char* t = "[Y]";
	pos = s.find(t);
	if (pos != -1) {
		s.replace(pos, strlen(t), itoa(tm->tm_year, tmp, 10));
	}

	t = "[M]";
	pos = s.find(t);
	if (pos != -1) {
		s.replace(pos, strlen(t), itoa(tm->tm_mon, tmp, 10));
	}

	t = "[D]";
	pos = s.find(t);
	if (pos != -1) {
		s.replace(pos, strlen(t), itoa(tm->tm_mday, tmp, 10));
	}

	t = "[h]";
	pos = s.find(t);
	if (pos != -1) {
		s.replace(pos, strlen(t), itoa(tm->tm_hour, tmp, 10));
	}

	t = "[m]";
	pos = s.find(t);
	if (pos != -1) {
		s.replace(pos, strlen(t), itoa(tm->tm_min, tmp, 10));
	}

	t = "[s]";
	pos = s.find(t);
	if (pos != -1) {
		s.replace(pos, strlen(t), itoa(tm->tm_sec, tmp, 10));
	}
	return s;
}