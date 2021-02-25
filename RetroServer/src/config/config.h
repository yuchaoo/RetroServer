#pragma once
#include <string>
#include <unordered_map>
#include "luaext/GameLua.h"
#include "luaext/LuaWrapper.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

typedef long long int64_t;


class MapKV {
public:
	enum Type
	{
		NONE,
		INT,
		BOOL,
		FLOAT,
		DOUBLE,
		UNSIGNED,
		INT64,
		STRING,
		MAP
	};
	MapKV();
	MapKV(int value);
	MapKV(bool value);
	MapKV(float value);
	MapKV(double value);
	MapKV(unsigned value);
	MapKV(int64_t value);
	MapKV(const char* value);
	MapKV(const std::string& value);
	MapKV(const MapKV& value);
	MapKV(MapKV&& value);
	MapKV(const std::unordered_map<MapKV, MapKV>& value);
	~MapKV();

	void SetType(Type type);
	int GetInt() const;
	unsigned GetUnsigned() const;
	bool GetBool() const;
	float GetFloat() const;
	double GetDouble() const;
	int64_t GetInt64() const;
	const char* GetString() const;
	const MapKV& GetValue(const MapKV& key) const;

	void SetAsMap();
	std::unordered_map<MapKV, MapKV>* GetMap();

	void operator = (int value);
	void operator = (bool value);
	void operator = (float value);
	void operator = (double value);
	void operator = (unsigned valuae);
	void operator = (int64_t value);
	void operator = (const char* value);
	void operator = (const std::string& value);
	void operator = (const MapKV& value);
	void operator = (const std::unordered_map<MapKV, MapKV>& value);
	void operator = (MapKV&& value);

	bool operator == (const MapKV& value) const;
private:
	union ShortType{
		int64_t i64;
		float fv;
		double dv;
		unsigned uv;
		int iv;
		bool bv;
		std::string* str;
		std::unordered_map<MapKV, MapKV>* map;
	} _short;
	Type _type;
	friend struct std::hash<MapKV>;
};

namespace std {
	template <>
	class hash<MapKV> {
	public:
		size_t operator()(const MapKV& v) const {
			if (v._type == MapKV::Type::INT) {
				return hash<int>()(v.GetInt());
			}
			else if (v._type == MapKV::Type::BOOL) {
				return hash<bool>()(v.GetBool());
			}
			else if (v._type == MapKV::Type::DOUBLE) {
				return hash<double>()(v.GetDouble());
			}
			else if (v._type == MapKV::Type::FLOAT) {
				return hash<float>()(v.GetFloat());
			}
			else if (v._type == MapKV::Type::INT64) {
				return hash<int64_t>()(v.GetInt64());
			}
			else if (v._type == MapKV::Type::STRING) {
				return hash<string>()(v.GetString());
			}
			else if (v._type == MapKV::Type::MAP) {
				return (size_t)v._short.map;
			}
			else if (v._type == MapKV::Type::UNSIGNED) {
				return hash<unsigned>()(v.GetUnsigned());
			}
			return 0;
		}
	};
};

void Lua_Unpack(lua_State* L, MapKV& value);
void Lua_Unpack(lua_State* L, std::unordered_map<MapKV, MapKV>& map);