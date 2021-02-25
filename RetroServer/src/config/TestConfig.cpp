
#include "TestConfig.h"

std::unordered_map<int, TestConfigItem*> TestConfig::_items = {};
TestConfigItem* TestConfig::Get(int index) {
	auto iter = _items.find(index);
	if (iter != _items.end()) {
		return iter->second;
	}
	lua_State* L = GameLua::GetInstance()->GetLuaState();
	int ret = Lua_GlobalTop(L, "TestConfigItem", index);
	if (ret != 2) {
		lua_pop(L, ret);
		return NULL;
	}
	TestConfigItem* item = new TestConfigItem;
	
	Lua_GetField(L, -1, "id");
	Lua_Unpack(L, item->id);

	Lua_GetField(L, -1, "name");
	Lua_Unpack(L, item->name);

	Lua_GetField(L, -1, "age");
	Lua_Unpack(L, item->age);

	Lua_GetField(L, -1, "sex");
	Lua_Unpack(L, item->sex);

	Lua_GetField(L, -1, "other");
	Lua_Unpack(L, item->other);

	lua_pop(L, ret);
	_items.insert(std::make_pair(index, item));
	return item;
}
void TestConfig::Clear() {
	for (auto iter = _items.begin(); iter != _items.end(); ++iter) {
		delete iter->second;
	}
	_items.clear();
}
