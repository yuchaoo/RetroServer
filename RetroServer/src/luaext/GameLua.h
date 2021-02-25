#pragma once
#include<string>
#include<share.h>
#include<memory>
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class GameLua
{
public:
    static GameLua* GetInstance();
    lua_State* GetLuaState();
    bool Init();
    void AddLuaPath(const char* dirpath);
    void SetLuaLoader(lua_CFunction fn, int index);
	void SetLuaFunc();
    bool LuaMain(const char* luafile);
    void Clear();
private:

    GameLua();
    ~GameLua();
private:
    lua_State* m_L;
    std::string m_luapath;
};