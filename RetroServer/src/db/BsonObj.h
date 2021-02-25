#pragma once
#include "mongoc.h"
#include <string>
#include <memory>
#include <vector>
#include "LogUtils.h"
#include "base/Ref.h"

void BsonIterKey(bson_iter_t* iter, int& key);
void BsonIterKey(bson_iter_t* iter, unsigned int& key);
void BsonIterKey(bson_iter_t* iter, std::string& key);

void BsonIterValue(bson_iter_t* iter, int& value);
void BsonIterValue(bson_iter_t* iter, unsigned int& value);
void BsonIterValue(bson_iter_t* iter, long& value);
void BsonIterValue(bson_iter_t* iter, int64_t& value);
void BsonIterValue(bson_iter_t* iter, uint64_t& value);
void BsonIterValue(bson_iter_t* iter, std::string& value);
void BsonIterValue(bson_iter_t* iter, bool& value);

class BsonObj;

class BsonIter {
public:
	BsonIter(BsonObj* obj);
	BsonIter(const BsonIter& iter);
	~BsonIter();

	void operator = (const BsonIter& iter);
	bool operator++ ();
	bool operator++ (int);
	bool operator !();
	operator bool();

	template<class Key, class Value>
	void value(Key& key, Value& value) {
		if (_bValid) {
			BsonIterKey(&_iter, key);
			BsonIterValue(&_iter, value);
		}
	}
private:
	bson_iter_t _iter;
	bool _bValid;
};

class BsonObj : public Ref {
public:
	BsonObj();
	~BsonObj();
	BsonObj(const bson_t* bson);
	BsonObj(const BsonObj& obj);
	BsonObj(uint8_t* data, uint32_t len);

	void Append(const char* key, int value);
	void Append(const char* key, int64_t value);
	void Append(const char* key, const char* value);
	void Append(const char* key, bool value);
	void Append(const char* key, float value);
	void Append(const char* key, double value);
	void Append(const char* key, long value);
	void Append(const char* key, unsigned int value);
	void Append(const char* key, const char* value, size_t len);
	void Append(const char* key, const std::string& value);

	void Append(int index, int value);
	void Append(int index, int64_t value);
	void Append(int index, const char* value);
	void Append(int index, bool value);
	void Append(int index, float value);
	void Append(int index, double value);
	void Append(int index, long value);
	void Append(int index, unsigned int value);
	void Append(int index, const char* value, size_t len);
	void Append(int index, const std::string& value);

	void AppendJson(const char* key, const char* json);
	void AppendJson(int index, const char* json);
	void AppendArray(const char* key, BsonObj* array);
	void AppendArray(int index, BsonObj* array);
	void AppendDocument(const char* key, BsonObj* doc);
	void AppendDocument(int index, BsonObj* doc);

	template<class T>
	void AppendArray(const char* key, const std::vector<T>& array) {
		BsonObj obj;
		for (size_t i = 0; i < array.size(); ++i) {
			obj.Append(i, array[i]);
		}
		bson_append_array(_bson, key, -1, obj._bson);
	}

	template<class T>
	void AppendArray(int index, const std::vector<T>& array) {
		static char buf[64];
		BsonObj obj;
		for (size_t i = 0; i < array.size(); ++i) {
			obj.Append(i, array[i]);
		}
		bson_append_array(_bson, itoa(index, buf, 10), -1, obj._bson);
	}

	int32_t Extract(const char* key, int32_t def);
	int64_t Extract(const char* key, int64_t def);
	std::string Extract(const char* key, const std::string& def);
	std::string Extract(const char* key, const char* def);
	bool Extract(const char* key, bool def);
	float Extract(const char* key, float def);
	double Extract(const char* key, double def);
	std::shared_ptr<BsonObj> ExtractArray(const char* key);
	std::shared_ptr<BsonObj> ExtractDocument(const char* key);
	
	int32_t Extract(int index, int32_t def);
	int64_t Extract(int index, int64_t def);
	bool Extract(int index, bool def);
	float Extract(int index, float def);
	double Extract(int index, double def);
	std::string Extract(int index, const std::string& def);
	std::string Extract(int index, const char* def);
	std::shared_ptr<BsonObj> ExtractArray(int index);
	std::shared_ptr<BsonObj> ExtractDocument(int index);

	template<class T>
	void ExtractArray(const char* key, std::vector<T>& result) {
		bson_iter_t iter;
		uint32_t size = 0;
		uint8_t* data = NULL;

		bson_iter_init(&iter, _bson);
		if (!bson_iter_find(&iter, key)) {
			LogUtils::LogDebug("can not find the key:%s", key);
			return;
		}
		bson_iter_array(&iter, &size, (const uint8_t**)&data);
		if (size > 0 && data) {
			return;
		}
		while (bson_iter_next(&iter)) {
			T t;
			BsonIterValue(&iter, t);
			result.emplace_back(t);
		}
	}

	template<class T>
	void ExtractArray(int index, std::vector<T>& result) {
		bson_iter_t iter;
		uint32_t size = 0;
		uint8_t* data = NULL;
		static char buf[64];

		bson_iter_init(&iter, _bson);
		if (!bson_iter_find(&iter, itoa(index, buf, 10))) {
			LogUtils::LogDebug("can not find the index:%d", index);
			return;
		}
		bson_iter_array(&iter, &size, (const uint8_t**)&data);
		if (size > 0 && data) {
			return;
		}
		while (bson_iter_next(&iter)) {
			T t;
			BsonIterValue(&iter, t);
			result.emplace_back(t);
		}
	}

	std::string toJson() const;
	BsonIter GetIter();
private:
	bson_t* _bson;
	friend class DBCollection;
	friend class BsonIter;
};