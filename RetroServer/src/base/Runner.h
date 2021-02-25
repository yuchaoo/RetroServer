#pragma once
#include "HashList.h"
#include <chrono>

class Runner {
public:
	Runner() {
		_key = s_index++;
	}
	virtual ~Runner() {}
	int GetKey() const { return _key; }
	virtual void Update(double escape) {};
	virtual void Second(double escape) {};
private:
	static int s_index;
private:
	int _key;
};

class RunnerPool {
public:
	static RunnerPool* GetInstance();
	void Init();
	void AddRunner(Runner* runner);
	void RemoveRunner(Runner* runnder);
	void Run();
private:
	RunnerPool();
	~RunnerPool();
private:
	HashList<Runner> _runners;
	std::vector<Runner*> _removeList;
	std::vector<Runner*> _addList;
	int _bRunning;
	double _curTime;
};