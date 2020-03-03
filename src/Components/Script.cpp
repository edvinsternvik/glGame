#include "Script.h"
#include <lua/src/lua.hpp>

namespace glGame {

    bool checkLua(lua_State* luaState, int result) {
        if(result != LUA_OK) {
            std::string errormsg = lua_tostring(luaState, -1);
            std::cout << errormsg << std::endl;
            return false;
        }

        return true;
    }

    Script::Script() : filename("test") {
        addPublicVariable((void*)filename, PublicVariableType::String, "scriptfile");

        m_luaState = luaL_newstate();
        luaL_openlibs(m_luaState);

        if(checkLua(m_luaState, luaL_dofile(m_luaState, "test.lua"))) {
            lua_getglobal(m_luaState, "init");
            if(lua_isfunction(m_luaState, -1)) {
                m_luaState, lua_pcall(m_luaState, 0, 0, 0);
            }
        }
        else {
            lua_close(m_luaState);
        }
    }

    Script::~Script() {
        lua_close(m_luaState);
    }

    void Script::update() {
        
    }

}