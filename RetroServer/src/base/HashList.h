#pragma once
#include <list>
#include <unordered_map>
#include "LogUtils.h"

template<class T>
class HashList {
public:
	struct Node {
		T* obj;
		Node* pre;
		Node* nex;
		Node():obj(NULL),pre(NULL),nex(NULL) {}
	};

	class Iter {
	public:
		Iter():_header(NULL),_next(NULL) {

		}
		Iter(HashList* hv) {
			_header = hv->_header;
			_next = NULL;
		}
		Iter(HashList* hv, Node* nex) {
			_header = hv->_header;
			_next = nex;
		}
		Iter(const Iter& iter) {
			_header = iter->_header;
			_next = iter->_next;
		}
		~Iter() {

		}
		bool operator++() {
			_next = _next ? _next->nex : _header;
			return _next;
		}
		bool operator++(int) {
			_next = _next ? _next->nex : _header;
			return _next;
		}
		void operator=(const Iter& iter) {
			_header = iter->_header;
			_next = iter->_next;
		}
		operator bool() {
			return _next == NULL;
		}
		T* operator*() {
			if (_next) {
				return _next->obj;
			}
			return NULL;
		}
	private:
		Node* _header;
		Node* _next;
		friend class HashList;
	};

	HashList() 
		:_header(NULL)
		, _tail(NULL) {

	}
	~HashList() {
		while (_header) {
			Node* nex = _header->nex;
			delete _header;
			_header = nex;
		}
	}

	void AddObj(int key, T* obj) {
		Assert(_map.find(key) != _map.end(), "the hash vec has the key");
		Node* node = new Node();
		node->obj = obj;
		node->pre = _tail;
		_tail = node;
		if (!_header) {
			_header = node;
		}
		_map.insert(std::make_pair(key, node));
	}
	void RemoveObj(int key) {
		auto iter = _map.find(key);
		if (iter != _map.end()) {
			Node* nex = iter->second->nex;
			Node* pre = iter->second->pre;
			if (nex) {
				nex->pre = pre;
			}
			if (pre) {
				pre->nex = nex;
			}
			delete iter->second;
			_map.erase(iter);
		}
	}

	Iter RemoveIter(const Iter& iter) {
		if (!iter->_header) {
			return Iter();
		}
		Node* nex = iter->_header ? iter->_header->nex : NULL;
		RemoveObj(iter->_header);
		return Iter(this, nex);
	}

	Iter GetIter() {
		return Iter(this);
	}
private:
	Node* _header;
	Node* _tail;
	std::unordered_map<int, Node*> _map;
	friend class Iter;
};