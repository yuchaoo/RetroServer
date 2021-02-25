#pragma once
#include <vector>

class Ref {
public:
	Ref();
	virtual ~Ref();
	void AutoRelease();
	void Retain();
	void Release();
	int GetRefCount();
public:
	int _luaID;
private:
	int _refCount;
};

class RefPool {
public:
	static RefPool* GetInstance();
	void AddRef(Ref* ref);
	void Update();
private:
	RefPool();
	~RefPool();
private:
	std::vector<Ref*> _refs;
};