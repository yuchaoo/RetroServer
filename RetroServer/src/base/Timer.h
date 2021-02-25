#pragma once
#include "Runner.h"
#include <functional>
typedef std::function<void()> TIMER_CALLBACK;

class Timer : public Runner {
public:
	Timer(float interval , float delay, int count, TIMER_CALLBACK handler);
	~Timer();
	void SetCallBack(TIMER_CALLBACK handler);
	virtual void Update(double curTime);
private:
	double _curTime;
	float _delayTime;
	float _interval;
	int _count;
	TIMER_CALLBACK _callback;
	bool _running;
	double _escape;
	double _startTime;
};