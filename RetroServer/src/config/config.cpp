#include "config.h"

MapKV::MapKV() 
:_type(Type::NONE){
	memset(&_short, 0, sizeof(_short));
}

MapKV::MapKV(int value):_type(Type::NONE){
	_short.iv = value;
	_type = Type::INT;
}

MapKV::MapKV(bool value) : _type(Type::NONE){
	_short.bv = value;
	_type = Type::BOOL;
}

MapKV::MapKV(float value) : _type(Type::NONE){
	_short.fv = value;
	_type = Type::FLOAT;
}

MapKV::MapKV(double value) :_type(Type::NONE){
	_short.dv = value;
	_type = Type::DOUBLE;
}

MapKV::MapKV(unsigned value) : _type(Type::NONE) {
	_short.uv = value;
	_type = Type::UNSIGNED;
}

MapKV::MapKV(int64_t value) : _type(Type::NONE){
	_short.i64 = value;
	_type = Type::INT64;
}

MapKV::MapKV(const char* value) : _type(Type::NONE){
	_short.str = new std::string(value);
	_type = Type::STRING;
}

MapKV::MapKV(const std::string& value) : _type(Type::NONE) {
	_short.str = new std::string(value);
	_type = Type::STRING;
}

MapKV::MapKV(const MapKV& value) {
	if (value._type == Type::STRING) {
		_short.str = new std::string( *value._short.str );
	}
	else if (value._type == Type::MAP) {
		_short.map = new std::unordered_map<MapKV, MapKV>( *value._short.map );
	}
	else {
		_short = value._short;
	}
	_type = value._type;
}

MapKV::MapKV(MapKV&& value) {
	if (_type == Type::STRING) {
		delete _short.str;
	}
	else if (_type == Type::MAP) {
		delete _short.map;
	}
	_short = value._short;
	_type = value._type;
	value.SetType(Type::NONE);
}

MapKV::MapKV(const std::unordered_map<MapKV, MapKV>& value) {
	_short.map = new std::unordered_map<MapKV, MapKV>(value);
	_type = Type::MAP;
}

MapKV::~MapKV() {
	if (_type == Type::STRING) {
		delete _short.str;
	}
	else if (_type == Type::MAP) {
		delete _short.map;
	}
}

void MapKV::SetType(Type type) {
	_type = type;
}

int MapKV::GetInt() const {
	if (_type == Type::INT) {
		return _short.iv;
	}
	return 0;
}

unsigned MapKV::GetUnsigned() const {
	if (_type == Type::UNSIGNED) {
		return _short.uv;
	}
	return 0;
}

bool MapKV::GetBool() const {
	if (_type == Type::BOOL) {
		return _short.bv;
	}
	return false;
}

float MapKV::GetFloat() const {
	if (_type == Type::FLOAT) {
		return _short.fv;
	}
	return 0;
}

double MapKV::GetDouble() const {
	if (_type == Type::DOUBLE) {
		return _short.dv;
	}
	return 0;
}

int64_t MapKV::GetInt64() const {
	if (_type == Type::INT64) {
		return _short.i64;
	}
	return 0;
}

const char* MapKV::GetString() const {
	if (_type == Type::STRING) {
		return _short.str->c_str();
	}
	return "";
}

const MapKV& MapKV::GetValue(const MapKV& key) const {
	if (_type == Type::MAP) {
		auto iter = _short.map->find(key);
		if (iter != _short.map->end()) {
			return iter->second;
		}
	}
	static MapKV def;
	return def;
}

void MapKV::SetAsMap() {
	if (_type == Type::MAP) {
		_short.map->clear();
		return;
	}
	if (_type == Type::STRING) {
		delete _short.str;
	}
	_short.map = new std::unordered_map<MapKV, MapKV>();
	_type = Type::MAP;
}

std::unordered_map<MapKV, MapKV>* MapKV::GetMap() {
	if (_type == Type::MAP) {
		return _short.map;
	}
	return NULL;
}

void MapKV::operator = (int value) {
	if (_type == Type::STRING) {
		delete _short.str;
	}
	else if (_type == Type::MAP) {
		delete _short.map;
	}
	_short.iv = value;
	_type = Type::INT;
}

void MapKV::operator = (bool value) {
	if (_type == Type::STRING) {
		delete _short.str;
	}
	else if (_type == Type::MAP) {
		delete _short.map;
	}
	_short.bv = value;
	_type = Type::BOOL;
}

void MapKV::operator = (float value) {
	if (_type == Type::STRING) {
		delete _short.str;
	}
	else if (_type == Type::MAP) {
		delete _short.map;
	}
	_short.fv = value;
	_type = Type::FLOAT;
}

void MapKV::operator = (double value) {
	if (_type == Type::STRING) {
		delete _short.str;
	}
	else if (_type == Type::MAP) {
		delete _short.map;
	}
	_short.dv = value;
	_type = Type::DOUBLE;
}

void MapKV::operator = (unsigned value) {
	if (_type == Type::STRING) {
		delete _short.str;
	}
	else if (_type == Type::MAP) {
		delete _short.map;
	}
	_short.uv = value;
	_type = Type::UNSIGNED;
}

void MapKV::operator = (int64_t value) {
	if (_type == Type::STRING) {
		delete _short.str;
	}
	else if (_type == Type::MAP) {
		delete _short.map;
	}
	_short.i64 = value;
	_type = Type::INT64;
}

void MapKV::operator = (const char* value) {
	if (_type == Type::STRING) {
		*_short.str = value;
	}
	else if(_type == Type::MAP){
		delete _short.map;
		_short.str = new std::string(value);
		_type = Type::STRING;
	}
	else {
		_short.str = new std::string(value);
		_type = Type::STRING;
	}
}

void MapKV::operator = (const std::string& value) {
	if (_type == Type::STRING) {
		*_short.str = value;
	}
	else if (_type == Type::MAP) {
		delete _short.map;
		_short.str = new std::string(value);
		_type = Type::STRING;
	}
	else {
		_short.str = new std::string(value);
		_type = Type::STRING;
	}
}

void MapKV::operator = (const MapKV& value) {
	if (_type == Type::STRING) {
		if (value._type == _type) {
			_short.str = value._short.str;
			return;
		}
		delete _short.str;
	}
	else if (_type == Type::MAP) {
		if (value._type == _type) {
			_short.map = value._short.map;
			return;
		}
		delete _short.map;
	}

	if (value._type == Type::STRING) {
		_short.str = new std::string(*value._short.str);
	}
	else if (value._type == Type::MAP) {
		_short.map = new std::unordered_map<MapKV,MapKV>(*value._short.map);
	}
	else {
		_short = value._short;
	}
	_type = value._type;
}

void MapKV::operator = (MapKV&& value) {
	if (_type == Type::STRING) {
		delete _short.str;
	}
	else if (_type == Type::MAP) {
		delete _short.map;
	}
	_short = value._short;
	_type = value._type;
	value.SetType(Type::NONE);
}

void MapKV::operator = (const std::unordered_map<MapKV, MapKV>& value) {
	if (_type == Type::MAP) {
		*_short.map = value;
	}
	else {
		if (_type == Type::STRING) {
			delete _short.str;
		}
		_short.map = new std::unordered_map<MapKV, MapKV>(value);
		_type = Type::MAP;
	}
}

bool MapKV::operator == (const MapKV& value) const {
	if (_type != value._type) {
		return false;
	}
	if (_type == Type::INT) {
		return _short.iv == value._short.iv;
	}
	if (_type == Type::UNSIGNED) {
		return _short.uv == value._short.uv;
	}
	if (_type == Type::BOOL) {
		return _short.bv == value._short.bv;
	}
	if (_type == Type::DOUBLE) {
		return _short.dv == value._short.dv;
	}
	if (_type == Type::FLOAT) {
		return _short.fv == value._short.fv;
	}
	if (_type == Type::INT64) {
		return _short.i64 == value._short.i64;
	}
	if (_type == Type::STRING) {
		return *_short.str == *value._short.str;
	}
	if (_type == Type::MAP) {
		return *_short.map == *value._short.map;
	}
	return false;
}

void Lua_Unpack(lua_State* L, MapKV& value) {
	if (lua_isstring(L,-1)) {
		value = lua_tostring(L, -1);
	}
	else if (lua_isinteger(L, -1)) {
		value = lua_tointeger(L, -1);
	}
	else if (lua_isboolean(L, -1)) {
		value = lua_toboolean(L, -1);
	}
	else if (lua_isnumber(L, -1)) {
		value = lua_tonumber(L, -1);
	}
	else if (lua_istable(L, -1)) {
		value.SetAsMap();
		Lua_Unpack(L, *value.GetMap());
	}
}

void Lua_Unpack(lua_State* L, std::unordered_map<MapKV, MapKV>& map) {
	if (!lua_istable(L, -1)) {
		return;
	}
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		MapKV value, key;
		Lua_Unpack(L, value);
		lua_pop(L, 1);
		Lua_Unpack(L, key);
		map[key] = std::move(value);
	}
}