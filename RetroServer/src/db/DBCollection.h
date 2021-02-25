#pragma once
#include "mongoc.h"
#include "MongoDB.h"
#include <string>
#include <memory>
#include <mutex>

class BsonObj;
typedef std::shared_ptr<BsonObj> BSONOBJ_PTR;
#define BSONOBJ_PTR_NEW(arg) BSONOBJ_PTR(new BsonObj(arg))

class DBCollection {
public:
	DBCollection(MongoDB* db, const char* name);
	~DBCollection();
	bool Init();
	bool Insert(BsonObj* bson);
	bool Insert(const std::vector<BsonObj*>& bsons);
	bool Query(BsonObj* bson, std::vector<BSONOBJ_PTR>& result);
	bool Query(BsonObj* filter, BsonObj* opts, std::vector<BSONOBJ_PTR>& result);
	bool Modify(BsonObj* filter, BsonObj* update, bool bNew);
	bool Update(BsonObj* selector, BsonObj* update);
	bool Update(BsonObj* selector, BsonObj* update, BsonObj* opts);
	bool Delete(BsonObj* selector);
	bool Delete(const std::vector<BsonObj*>& bsons);
private:
	std::string _name;
	MongoDB* _db;
	mongoc_collection_t* _collection;
	std::mutex _executeMutex;
};