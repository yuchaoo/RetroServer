
#include <string>
#include <unordered_map>
#include "luaext/GameLua.h"
#include "luaext/LuaWrapper.h"
#include "config.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

struct TestConfigItem{

	int id;

	std::string name;

	int age;

	int sex;

	std::unordered_map<MapKV,MapKV> other;

};

class TestConfig {
public:
	static TestConfigItem* Get(int index);
	static void Clear();
private:
	static std::unordered_map<int, TestConfigItem*> _items;
};
