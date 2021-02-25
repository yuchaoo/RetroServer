#include "DBCollection.h"
#include "LogUtils.h"
#include "BsonObj.h"

DBCollection::DBCollection(MongoDB* db, const char* name) 
:_db(db)
,_name(name)
, _collection(NULL){

}

DBCollection::~DBCollection() {
	if (_collection) {
		mongoc_collection_destroy(_collection);
	}
}

bool DBCollection::Init() {
	if (!_db) {
		LogUtils::LogError("init collection : %s failed", _name.c_str());
		return false;
	}
	_collection = mongoc_client_get_collection(_db->GetClient(), _db->GetName().c_str(), _name.c_str());
	if (!_collection) {
		LogUtils::LogError("create collection %s failed", _name.c_str());
		return false;
	}
	return true;
}

bool DBCollection::Insert(BsonObj* bson) {
	std::lock_guard<std::mutex> lock(_executeMutex);
	bson_error_t error;
	bson_t reply;
	bool ret = mongoc_collection_insert_one(_collection, bson->_bson, NULL,&reply, &error);
	if (!ret) {
		LogUtils::LogWarning("insert bson failed, the json:%s, error:%s", bson_as_json(bson->_bson, NULL), error.message);
		return false;
	}
	return true;
}

bool DBCollection::Insert(const std::vector<BsonObj*>& bsons) {
	std::lock_guard<std::mutex> lock(_executeMutex);

	bson_error_t error;
	bson_t reply = BSON_INITIALIZER;
	if (bsons.size() <= 0) {
		return true;
	}

	std::vector<bson_t*> list(bsons.size());
	for (size_t i = 0; i < bsons.size(); ++i) {
		list[i] = bsons[i]->_bson;
	}
	bool ret = mongoc_collection_insert_many(_collection, (const bson_t**)&list[0], list.size(), NULL, &reply, &error);
	if (!ret) {
		LogUtils::LogError("batch insert bsons failed, error:%s", error.message);
		return false;
	}
	return true;
}

bool DBCollection::Query(BsonObj* bson, std::vector<BSONOBJ_PTR>& result) {
	std::lock_guard<std::mutex> lock(_executeMutex);
	bson_error_t error;
	mongoc_cursor_t* cursor = NULL;
	const bson_t* doc;

	cursor = mongoc_collection_find(_collection, mongoc_query_flags_t::MONGOC_QUERY_NONE, 0, 0, 0, bson->_bson, NULL, NULL);
	while (mongoc_cursor_next(cursor, &doc)) {
		result.push_back(BSONOBJ_PTR_NEW(doc));
	}

	if (mongoc_cursor_error(cursor, &error)) {
		LogUtils::LogError("query error: %s\n", error.message);
		mongoc_cursor_destroy(cursor);
		return false;
	}

	mongoc_cursor_destroy(cursor);
	return true;
}

bool DBCollection::Query(BsonObj* filter, BsonObj* opts, std::vector<BSONOBJ_PTR>& result) {
	std::lock_guard<std::mutex> lock(_executeMutex);
	mongoc_cursor_t* cursor;
	bson_error_t error;
	const bson_t* doc;
	if (!filter) {
		LogUtils::LogError("query error, the filter is empty");
		return false;
	}
	cursor = mongoc_collection_find_with_opts(_collection, filter->_bson, opts->_bson, NULL);
	while (mongoc_cursor_next(cursor, &doc)) {
		result.push_back(BSONOBJ_PTR_NEW(doc));
	}

	if (mongoc_cursor_error(cursor, &error)) {
		LogUtils::LogError("query error: %s\n", error.message);
		mongoc_cursor_destroy(cursor);
		return false;
	}

	mongoc_cursor_destroy(cursor);
	return true;
}

bool DBCollection::Modify(BsonObj* filter, BsonObj* update, bool bNew) {
	Assert(filter,"modify : the filter is empty");
	Assert(update, "modify : the update is empty");
	std::lock_guard<std::mutex> lock(_executeMutex);
	bson_t reply = BSON_INITIALIZER;
	bson_error_t error;
	bool ret = mongoc_collection_find_and_modify(_collection, filter->_bson,NULL, update->_bson,NULL,false,false, bNew,&reply,&error);
	if (!ret) {
		LogUtils::LogError("modify : error %s", error.message);
		return false;
	}
	char* str = bson_as_canonical_extended_json(&reply, NULL);
	LogUtils::LogDebug("modify secceed: %s", str);
	bson_free(str);
	return true;
}

bool DBCollection::Update(BsonObj* selector, BsonObj* update) {
	Assert(selector, "Update : the selector is empty");
	Assert(update, "Update : the update is empty");
	std::lock_guard<std::mutex> lock(_executeMutex);
	bson_t reply = BSON_INITIALIZER;
	bson_error_t error;
	if (!mongoc_collection_update_one(_collection, selector->_bson, update->_bson, NULL, &reply, &error)) {
		LogUtils::LogError("update : error : %s",error.message);
		return false;
	}
	char* str = bson_as_canonical_extended_json(&reply, NULL);
	LogUtils::LogDebug("update secceed: %s", str);
	bson_free(str);
	return true;
}

bool DBCollection::Update(BsonObj* selector, BsonObj* update, BsonObj* opts) {
	Assert(selector, "Update : the select is empty");
	Assert(update, "Update : the update is empty");
	Assert(opts, "Update : opts is empty");
	std::lock_guard<std::mutex> lock(_executeMutex);
	bson_t reply = BSON_INITIALIZER;
	bson_error_t error;
	if (!mongoc_collection_update_many(_collection, selector->_bson, update->_bson, opts->_bson, &reply, &error)) {
		LogUtils::LogError("update : error %s", error.message);
		return false;
	}
	char* str = bson_as_canonical_extended_json(&reply, NULL);
	LogUtils::LogDebug("update secceed: %s", str);
	bson_free(str);
	return true;
}

bool DBCollection::Delete(BsonObj* selector) {
	Assert(selector, "Delete : the selector is empty");
	std::lock_guard<std::mutex> lock(_executeMutex);
	bson_t reply = BSON_INITIALIZER;
	bson_error_t error;
	if (!mongoc_collection_delete_one(_collection, selector->_bson, NULL, &reply, &error)) {
		LogUtils::LogError("delete : error:%s", error.message);
		return false;
	}
	char* str = bson_as_canonical_extended_json(&reply, NULL);
	LogUtils::LogDebug("delete secceed:%s", str);
	return true;
}

bool DBCollection::Delete(const std::vector<BsonObj*>& bsons) {
	std::lock_guard<std::mutex> lock(_executeMutex);
	if (bsons.size() <= 0) {
		return true;
	}
	bson_t reply = BSON_INITIALIZER;
	bson_error_t error;
	BsonObj obj;
	for (size_t i = 0; i < bsons.size(); ++i) {
		obj.AppendArray(i, bsons[i]);
	}
	if (!mongoc_collection_delete_many(_collection, obj._bson, NULL, &reply, &error)) {
		LogUtils::LogError("delete many : error:%s",error.message);
		return false;
	}
	char* str = bson_as_canonical_extended_json(&reply, NULL);
	LogUtils::LogDebug("delete secceed:%s", str);
	return true;
}

