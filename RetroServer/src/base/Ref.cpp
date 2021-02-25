#include "Ref.h"
#include "LogUtils.h"

Ref::Ref()
:_refCount(1)
, _luaID(0){

}

Ref::~Ref() {
	Assert(_refCount <= 0, "the Ref obj ref count is no 0");
}

void Ref::AutoRelease() {
	RefPool::GetInstance()->AddRef(this);
}

void Ref::Retain() {
	_refCount++;
}

void Ref::Release() {
	if (--_refCount <= 0) {
		delete this;
	}
}

int Ref::GetRefCount() {
	return _refCount;
}

/********************************************/

RefPool* RefPool::GetInstance() {
	static RefPool pool;
	return &pool;
}

RefPool::RefPool() {

}

RefPool::~RefPool() {

}

void RefPool::AddRef(Ref* ref) {
	_refs.push_back(ref);
}

void RefPool::Update() {
	for (size_t i = 0; i < _refs.size(); ++i) {
		_refs[i]->Release();
	}
	_refs.resize(0);
}

