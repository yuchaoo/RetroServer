#include "Timer.h"

Timer::Timer(float interval, float delay, int count , TIMER_CALLBACK handler)
:_interval(interval)
,_delayTime(delay)
,_callback(handler)
,_escape(_interval)
, _startTime(0)
,_count(count){
	RunnerPool::GetInstance()->AddRunner(this);
	_running = true;
}

Timer::~Timer() {
	if (!_running) {
		RunnerPool::GetInstance()->RemoveRunner(this);
	}
}

void Timer::SetCallBack(TIMER_CALLBACK handler) {
	_callback = handler;
}

void Timer::Update(double curTime) {
	if (_curTime == 0) {
		_curTime = curTime;
		_startTime = curTime;
	}
	_curTime = curTime;

	if (_curTime - _startTime <= _delayTime) {
		return;
	}

	double dt = curTime - _curTime;
	_escape -= dt;
	if (_escape <= 0) {
		if (_count > 0 || _count < 0) {
			if (_callback) {
				_callback();
			}
			_escape += _interval;
			if (_count > 0) {
				_count--;
			}
		}
		if (_count == 0) {
			_running = false;
			RunnerPool::GetInstance()->RemoveRunner(this);
		}
	}
}