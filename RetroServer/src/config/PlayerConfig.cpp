
#include "PlayerConfig.h"

std::unordered_map<std::string, PlayerConfigItem*> PlayerConfig::_items = {};
PlayerConfigItem* PlayerConfig::Get(std::string index) {
	auto iter = _items.find(index);
	if (iter != _items.end()) {
		return iter->second;
	}
	lua_State* L = GameLua::GetInstance()->GetLuaState();
	int ret = Lua_GlobalTop(L, "PlayerConfig", index);
	if (ret != 2) {
		lua_pop(L, ret);
		return NULL;
	}
	PlayerConfigItem* item = new PlayerConfigItem;
	
	Lua_GetField(L, -1, "id");
	Lua_Unpack(L, item->id);

	Lua_GetField(L, -1, "atk");
	Lua_Unpack(L, item->atk);

	Lua_GetField(L, -1, "atk_strength");
	Lua_Unpack(L, item->atk_strength);

	Lua_GetField(L, -1, "hujia");
	Lua_Unpack(L, item->hujia);

	Lua_GetField(L, -1, "magic");
	Lua_Unpack(L, item->magic);

	Lua_GetField(L, -1, "magic_strength");
	Lua_Unpack(L, item->magic_strength);

	Lua_GetField(L, -1, "magic_kangxing");
	Lua_Unpack(L, item->magic_kangxing);

	Lua_GetField(L, -1, "args");
	Lua_Unpack(L, item->args);

	lua_pop(L, ret);
	_items.insert(std::make_pair(index, item));
	return item;
}
void PlayerConfig::Clear() {
	for (auto iter = _items.begin(); iter != _items.end(); ++iter) {
		delete iter->second;
	}
	_items.clear();
}
