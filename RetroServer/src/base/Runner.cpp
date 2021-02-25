#include "Runner.h"
#include "TimeUtils.h"

int Runner::s_index = 0;

RunnerPool* RunnerPool::GetInstance() {
	static RunnerPool* pool = NULL;
	if (!pool) {
		pool = new RunnerPool();
		pool->Init();
	}
	return pool;
}

RunnerPool::RunnerPool()
:_bRunning(0){

}

RunnerPool::~RunnerPool() {

}

void RunnerPool::Init() {
	_curTime = TimeUtils::GetTimeOfDayMs() / 1000.0f;
}

void RunnerPool::AddRunner(Runner* runner) {
	if (_bRunning > 0) {
		_addList.push_back(runner);
	}
	else {
		_runners.AddObj(runner->GetKey(),runner);
	}
}

void RunnerPool::RemoveRunner(Runner* runner) {
	if (_bRunning > 0) {
		_removeList.push_back(runner);
	}
	else {
		_runners.RemoveObj(runner->GetKey());
	}
}

void RunnerPool::Run() {
	++_bRunning;
	double curTime = TimeUtils::GetTimeOfDayMs() / 1000.0;
	bool bAcross = (curTime - _curTime) >= 1.0;
	if (bAcross) {
		_curTime = curTime;
	}
	auto iter = _runners.GetIter();
	while (++iter) {
		(*iter)->Update(curTime);
		if (bAcross) {
			(*iter)->Second(curTime);
		}
	}
	--_bRunning;

	if (_bRunning <= 0) {
		if (_removeList.size() > 0) {
			for (size_t i = 0; i < _removeList.size(); ++i) {
				RemoveRunner(_removeList[i]);
			}
			_removeList.resize(0);
		}

		if (_addList.size() > 0) {
			for (size_t i = 0; i < _addList.size(); ++i) {
				AddRunner(_addList[i]);
			}
			_addList.resize(0);
		}
	}
}

