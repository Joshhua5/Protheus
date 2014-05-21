/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 21:05:2014: Waring J.
*************************************************************************/

#pragma once

namespace Pro{
	namespace Util{
		template<typename T> T* luaP_touserdata(lua_State* L, int idx){
			return *static_cast<T**>(lua_touserdata(L, idx));
		}

		// Creates a user data on the Lua stack, assuming that there's a name passed as the first argument
		// also assuming that the Class accepts a name for GUID generation
		template<typename T> T** luaP_newuserdata(lua_State* L){
			T** o = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
			*o = new T(lua_tostring(L, 1));
			return o;
		}

		static void luaP_setmetatable(lua_State* L, const char* metatable, int idx){
			luaL_getmetatable(L, metatable);
			lua_setmetatable(L, -2);
		}

		static void luaP_registerstore(lua_State* L, const std::string& key, void* data){
			lua_pushlightuserdata(L, (void*)&key[0]);
			lua_pushlightuserdata(L, data);
			lua_settable(L, LUA_REGISTRYINDEX);
		}

		template<typename T> T* luaP_registerget(lua_State* L, const std::string& key){
			lua_pushlightuserdata(L, (void*)&key[0]);
			lua_settable(L, LUA_REGISTRYINDEX);
			return (T*)lua_touserdata(L, -1);
		}

		static int dumpLuaStack(lua_State *L) {
			int i;
			int top = lua_gettop(L);
			for (i = 1; i <= top; i++) {  /* repeat for each level */
				int t = lua_type(L, i);
				switch (t) {

				case LUA_TSTRING:  /* strings */
					printf("`%s'", lua_tostring(L, i));
					break;

				case LUA_TBOOLEAN:  /* booleans */
					printf(lua_toboolean(L, i) ? "true" : "false");
					break;

				case LUA_TNUMBER:  /* numbers */
					printf("%g", lua_tonumber(L, i));
					break;

				default:  /* other values */
					printf("%s", lua_typename(L, t));
					break;

				}
				printf("  ");  /* put a separator */
			}
			printf("\n");  /* end the listing */
			return 0;
		}
	}
}