
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

struct PlayerConfigItem{

	std::string id;

	int atk;

	float atk_strength;

	int hujia;

	int magic;

	float magic_strength;

	int magic_kangxing;

	std::unordered_map<MapKV,MapKV> args;

};

class PlayerConfig {
public:
	static PlayerConfigItem* Get(std::string index);
	static void Clear();
private:
	static std::unordered_map<std::string, PlayerConfigItem*> _items;
};
