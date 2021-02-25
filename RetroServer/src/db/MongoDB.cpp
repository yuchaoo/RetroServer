#include "MongoDB.h"
#include "LogUtils.h"
#include "db/DBCollection.h"

MongoDB::MongoDB(mongoc_client_t* client, const char* name)
:_name(name)
,_client(client)
, _database(NULL){

}

MongoDB::~MongoDB() {
	if (_database) {
		mongoc_database_destroy(_database);
		_database = NULL;
	}
}

bool MongoDB::Init() {
	if (!_client) return false;
	_database = mongoc_client_get_database(_client, _name.c_str());
	if (!_database) {
		LogUtils::LogError("create database : %s failed", _name.c_str());
		return false;
	}
	return true;
}

DBCollection* MongoDB::CreateCollection(const char* name) {
	DBCollection* coll = new DBCollection(this, name);
	auto iter = _colMap.find(name);
	if (iter != _colMap.end()) {
		delete iter->second;
	}
	_colMap.insert(std::make_pair(name, coll));
	return coll;
}

DBCollection* MongoDB::GetCollection(const char* name) {
	auto iter = _colMap.find(name);
	if (iter != _colMap.end()) {
		return iter->second;
	}
	return NULL;
}

bool MongoDB::IsHasCollection(const char* name) {
	return _colMap.find(name) != _colMap.end();
}