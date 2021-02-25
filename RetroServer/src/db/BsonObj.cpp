#include "BsonObj.h"
#include "LogUtils.h"

static char buf[64];

void BsonIterKey(bson_iter_t* iter, int& key) {
	const char* s = bson_iter_key(iter);
	key = atoi(s);
}

void BsonIterKey(bson_iter_t* iter, unsigned int& key) {
	const char* s = bson_iter_key(iter);
	key = atoi(s);
}

void BsonIterKey(bson_iter_t* iter, std::string& key) {
	key = bson_iter_key(iter);
}

void BsonIterValue(bson_iter_t* iter, int& value) {
	value = bson_iter_int32(iter);
}

void BsonIterValue(bson_iter_t* iter, unsigned int& value) {
	value = bson_iter_int32(iter);
}

void BsonIterValue(bson_iter_t* iter, long& value) {
	value = bson_iter_int32(iter);
}

void BsonIterValue(bson_iter_t* iter, int64_t& value) {
	value = bson_iter_int64(iter);
}

void BsonIterValue(bson_iter_t* iter, uint64_t& value) {
	value = bson_iter_int64(iter);
}

void BsonIterValue(bson_iter_t* iter, std::string& value) {
	size_t size = 0;
	const char* str = bson_iter_utf8(iter, &size);
	if (str) {
		value.assign(str, 0, size);
	}
}

void BsonIterValue(bson_iter_t* iter, bool& value) {
	value = bson_iter_bool(iter);
}

/****************************************************/

BsonIter::BsonIter(BsonObj* obj)
:_bValid(false){
	bson_iter_init(&_iter, obj->_bson);
}

BsonIter::BsonIter(const BsonIter& iter) {
	_iter = iter._iter;
	_bValid = iter._bValid;
}

BsonIter::~BsonIter() {
}

void BsonIter::operator = (const BsonIter& iter) {
	_iter = iter._iter;
	_bValid = iter._bValid;
}

bool BsonIter::operator ++() {
	return _bValid = bson_iter_next(&_iter);
}

bool BsonIter::operator++ (int) {
	return _bValid = bson_iter_next(&_iter);
}
bool BsonIter::operator !() {
	return !_bValid;
}

BsonIter::operator bool() {
	return _bValid;
}


/*****************************************************/
BsonObj::BsonObj()
:_bson(NULL){
	_bson = bson_new();
	bson_init(_bson);
}

BsonObj::BsonObj(uint8_t* data, uint32_t len) {
	_bson = bson_new_from_data(data, len);
}

BsonObj::BsonObj(const BsonObj& obj) {
	_bson = bson_copy(obj._bson);
}

BsonObj::BsonObj(const bson_t* bson) {
	_bson = bson_copy(bson);
}

BsonObj::~BsonObj() {
	bson_destroy(_bson);
}

void BsonObj::Append(const char* key, int value) {
	bson_append_int32(_bson, key, strlen(key), value);
}

void BsonObj::Append(int index, int value) {
	bson_append_int32(_bson, itoa(index, buf, 10), -1, value);
}

void BsonObj::Append(const char* key, int64_t value) {
	bson_append_int64(_bson, key, strlen(key), value);
}

void BsonObj::Append(int index, int64_t value) {
	bson_append_int64(_bson, itoa(index, buf, 10), -1, value);
}

void BsonObj::Append(const char* key, const char* value) {
	bson_append_utf8(_bson, key, strlen(key), value, strlen(value));
}

void BsonObj::Append(int index, const char* value) {
	bson_append_utf8(_bson, itoa(index, buf, 10), -1, value, strlen(value));
}

void BsonObj::Append(const char* key, const char* value, size_t len) {
	bson_append_symbol(_bson, key, strlen(key), value, len);
}

void BsonObj::Append(const char* key, const std::string& value) {
	bson_append_utf8(_bson, key, strlen(key), value.c_str(), value.size());
}

void BsonObj::Append(int index, const char* value, size_t len) {
	bson_append_utf8(_bson, itoa(index, buf, 10), -1, value, len);
}

void BsonObj::Append(int index, const std::string& value) {
	bson_append_utf8(_bson, itoa(index, buf, 10), -1, value.c_str(), value.size());
}

void BsonObj::Append(const char* key, bool value) {
	bson_append_bool(_bson, key, strlen(key), value);
}

void BsonObj::Append(int index, bool value) {
	bson_append_bool(_bson, itoa(index, buf, 10), -1, value);
}

void BsonObj::Append(const char* key, float value) {
	bson_append_double(_bson, key, strlen(key), value);
}

void BsonObj::Append(int index, float value) {
	bson_append_double(_bson, itoa(index, buf, 10), -1, value);
}

void BsonObj::Append(const char* key, double value) {
	bson_append_double(_bson, key, strlen(key), value);
}

void BsonObj::Append(const char* key, long value) {
	bson_append_int32(_bson, key, strlen(key), value);
}

void BsonObj::Append(const char* key, unsigned int value) {
	bson_append_int32(_bson, key, strlen(key), value);
}

void BsonObj::Append(int index, double value) {
	bson_append_double(_bson, itoa(index, buf, 10), -1, value);
}

void BsonObj::Append(int index, long value) {
	bson_append_int32(_bson, itoa(index, buf, 10), -1, value);
}

void BsonObj::Append(int index, unsigned int value) {
	bson_append_int32(_bson, itoa(index, buf, 10), -1, value);
}

void BsonObj::AppendJson(const char* key, const char* json) {
	bson_t* bson;
	bson_error_t error;
	bson = bson_new_from_json((const uint8_t*)json,strlen(json),&error);
	if (!bson) {
		LogUtils::LogError("AppendJson failed, error:%s", error.message);
		return;
	}
	bson_append_document(_bson, key, strlen(key), bson);
	bson_destroy(bson);
}

void BsonObj::AppendJson(int index, const char* json) {
	bson_t* bson;
	bson_error_t error;
	bson = bson_new_from_json((const uint8_t*)json, strlen(json), &error);
	if (!bson) {
		LogUtils::LogError("AppendJson failed, error:%s", error.message);
		return;
	}
	bson_append_document(_bson, itoa(index,buf,10), -1, bson);
	bson_destroy(bson);
}

void BsonObj::AppendArray(const char* key, BsonObj* array) {
	if (!bson_append_array(_bson, key, strlen(key), array->_bson)) {
		LogUtils::LogError("append doc failed, key:%s", key);
	}
}

void BsonObj::AppendArray(int index, BsonObj* array) {
	if (!bson_append_array(_bson, itoa(index, buf, 10), -1, array->_bson)) {
		LogUtils::LogError("append doc failed, index:%d", index);
	}
}

void BsonObj::AppendDocument(const char* key, BsonObj* doc) {
	if (!bson_append_document(_bson, key, strlen(key), doc->_bson)) {
		LogUtils::LogError("append doc failed, key:%s",key);
	}
}

void BsonObj::AppendDocument(int index, BsonObj* doc) {
	if (!bson_append_document(_bson, itoa(index, buf, 10), -1, doc->_bson)) {
		LogUtils::LogError("append doc failed, index:%d", index);
	}
}

int32_t BsonObj::Extract(const char* key, int32_t def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, key)) {
		return bson_iter_int32(&iter);
	}
	return def;
}

int64_t BsonObj::Extract(const char* key, int64_t def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, key)) {
		return bson_iter_int32(&iter);
	}
	return def;
}

std::string BsonObj::Extract(const char* key, const std::string& def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, key)) {
		uint32_t len = 0;
		const char* str = bson_iter_utf8(&iter, &len);
		return std::string(str, len);
	}
	return def;
}

std::string BsonObj::Extract(const char* key, const char* def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, key)) {
		uint32_t len = 0;
		const char* str = bson_iter_utf8(&iter, &len);
		return std::string(str, len);
	}
	return def;
}

bool BsonObj::Extract(const char* key, bool def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, key)) {
		return bson_iter_bool(&iter);
	}
	return def;
}

float BsonObj::Extract(const char* key, float def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, key)) {
		return (float)bson_iter_double(&iter);
	}
	return def;
}

double BsonObj::Extract(const char* key, double def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, key)) {
		return bson_iter_double(&iter);
	}
	return def;
}

std::shared_ptr<BsonObj> BsonObj::ExtractArray(const char* key) {
	bson_iter_t iter;
	size_t size = 0;
	uint8_t* data = NULL;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, key)) {
		bson_iter_array(&iter, &size, (const uint8_t**)&data);
	}
	if (size == 0) {
		return NULL;
	}
	return std::shared_ptr<BsonObj>(new BsonObj(data, size));
}

std::shared_ptr<BsonObj> BsonObj::ExtractDocument(const char* key) {
	bson_iter_t iter;
	uint32_t size = 0;
	uint8_t* data = NULL;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, key)) {
		bson_iter_document(&iter, &size, (const uint8_t**)&data);
	}
	if (size == 0) {
		return NULL;
	}
	return std::shared_ptr<BsonObj>(new BsonObj(data, size));
}

int32_t BsonObj::Extract(int index, int32_t def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, itoa(index, buf, 10))) {
		return bson_iter_int32(&iter);
	}
	return def;
}

int64_t BsonObj::Extract(int index, int64_t def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, itoa(index, buf, 10))) {
		return bson_iter_int64(&iter);
	}
	return def;
}

std::string BsonObj::Extract(int index, const std::string& def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, itoa(index, buf, 10))) {
		uint32_t len = 0;
		const char* str = bson_iter_symbol(&iter, &len);
		return std::string(str, len);
	}
	return def;
}

std::string BsonObj::Extract(int index, const char* def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, itoa(index, buf, 10))) {
		uint32_t len = 0;
		const char* str = bson_iter_symbol(&iter, &len);
		return std::string(str, len);
	}
	return def;
}

bool BsonObj::Extract(int index, bool def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, itoa(index, buf, 10))) {
		return bson_iter_bool(&iter);
	}
	return def;
}

float BsonObj::Extract(int index, float def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, itoa(index, buf, 10))) {
		return (float)bson_iter_double(&iter);
	}
	return def;
}

double BsonObj::Extract(int index, double def) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, itoa(index, buf, 10))) {
		return bson_iter_double(&iter);
	}
	return def;
}

std::shared_ptr<BsonObj> BsonObj::ExtractArray(int index) {
	bson_iter_t iter;
	size_t size = 0;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, itoa(index, buf, 10))) {
		uint8_t* data = NULL;
		bson_iter_array(&iter, &size, (const uint8_t**)&data);
		if (size > 0) {
			return std::shared_ptr<BsonObj>(new BsonObj(data, size));
		}
	}
	return NULL;
}

std::shared_ptr<BsonObj> BsonObj::ExtractDocument(int index) {
	bson_iter_t iter;
	bson_iter_init(&iter, _bson);
	if (bson_iter_find(&iter, itoa(index, buf, 10))) {
		uint32_t size = 0;
		uint8_t* data = NULL;
		bson_iter_document(&iter, &size, (const uint8_t**)&data);
		if (size > 0) {
			return std::shared_ptr<BsonObj>(new BsonObj(data, size));
		}
	}
	return NULL;
}

std::string BsonObj::toJson() const {
	size_t size = 0;
	char* str = bson_as_canonical_extended_json(_bson, &size);
	std::string s(str, size);
	bson_free(str);
	return s;
}

BsonIter BsonObj::GetIter() {
	return BsonIter(this);
}