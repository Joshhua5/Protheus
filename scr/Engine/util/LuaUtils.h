/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:
Provides a small wrapper around Lua to add functionality to simplify
intereaction with the stack and registry.

-------------------------------------------------------------------------
History:
- 21:05:2014: Waring J.
*************************************************************************/

#pragma once
#include "..\lua\lib\lua.hpp"
#include <vector>
#include <iostream>
using namespace std;

namespace Pro{
	namespace Util{

		inline void checkError(lua_State* L, int error){
			if (error)
				std::cout << "Lua Error: " << lua_tostring(L, -1) << std::endl;
		}

		template<typename T>
		struct luaP_array{
			vector<T> elements;
			void add(T& value){
				elements.push_back(value);
			}
		};

		// The first part of the pair will be used as the key and the second as the data
		template<typename T>
		inline void luaP_pushtoarray(lua_State* L, std::string table_name, char* key, T data){
			lua_pushstring(L, &table_name[0]);
			lua_gettable(L, -1);
			lua_pushstring(L, key); 
			lua_pushnumber(L, data);
			lua_settable(L, -3);
		}

		template<typename T>
		inline void luaP_pusharray(lua_State* L, vector<std::pair<std::string, T>>& elements){
			lua_createtable(L, 0, 0);
			for (int x = 0; x < elements.size(); x++){
				lua_pushstring(L, elements.at(x)[0]);
				lua_pushnumber(L, elements.at(x)[1]);
				lua_settable(L, -3);
			}
		} 

		template<typename T>
		inline void luaP_pusharray(lua_State* L, T* data, unsigned int size){  
			lua_createtable(L, 0, 0);
			for (unsigned int x = 0; x < size; x++){
				lua_pushnumber(L, x);
				lua_pushnumber(L, data[x]);
				lua_settable(L, -3);
			}
		}

		template<typename T> T* luaP_touserdata(lua_State* L, int idx){
			return *static_cast<T**>(lua_touserdata(L, idx));
		}

		inline void luaP_setmetatable(lua_State* L, const string& metatable){
			luaL_getmetatable(L, &metatable[0]);
			lua_setmetatable(L, -2);
		}

		// Creates a user data on the Lua stack, assuming that there's a name passed as the first argument
		// also assuming that the Class accepts a name for GUID generation

		template<typename T> T** luaP_newuserdata(lua_State* L, T* data){
			T** o = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
			// Does not copy the data, only remembers reference to it
			*o = data;
			return o;
		}

		// Automaticallys binds the metatable
		template<typename T> T** luaP_newobject(lua_State* L, T* data) {
			T** o = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
			*o = data; 
			luaP_setmetatable(L, T::lGetMetatable());
			return o;
		}

		inline void luaP_registerstore(lua_State* L, const std::string& key, void* data){
			lua_pushstring(L, &key[0]);
			lua_pushlightuserdata(L, data);
			lua_settable(L, LUA_REGISTRYINDEX);
		}

		inline void luaP_checkerror(lua_State* L, int error_code){
			if (error_code != LUA_OK){
				cout << lua_tostring(L, -1) << endl;
			}
		}

		template<typename T> T* luaP_registerget(lua_State* L, const std::string& key){
			lua_pushstring(L, &key[0]);
			lua_gettable(L, LUA_REGISTRYINDEX);
			auto pointer = static_cast<T*>(lua_touserdata(L, -1));
			lua_pop(L, 1);
			return pointer;

		}

		inline void luaP_dumpLuaStack(lua_State *L) {
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
		}



#define luaP_getFileSystem(lua_state) Pro::Util::luaP_registerget<Pro::Util::FileSystem>(lua_state, "FILESYSTEM")
#define luaP_setFileSystem(lua_state, data) Pro::Util::luaP_registerstore(lua_state, "FILESYSTEM", data)

#define luaP_setWindow(lua_state, data) Util::luaP_registerstore(lua_state, "WINDOW", data)
#define luaP_getWindow(lua_state) Util::luaP_registerget<Pro::Window>(lua_state, "WINDOW")

#define luaP_setRenderer(lua_state, data) Util::luaP_registerstore(lua_state, "RENDERER", data)
#define luaP_getRenderer(lua_state) Util::luaP_registerget<Pro::Graphics::Renderer>(lua_state, "RENDERER")

#define luaP_setSDLRenderer(lua_state, data) Util::luaP_registerstore(lua_state, "SDL_RENDERER", data)
#define	luaP_getSDLRenderer(lua_state) Util::luaP_registerget<SDL_Renderer>(lua_state, "SDL_RENDERER")

#define luaP_setScenes(lua_state, data) Util::luaP_registerstore(lua_state, "SCENES", data)
#define luaP_getScenes(lua_state) Util::luaP_registerget<Pro::SceneContainer>(lua_state, "SCENES")

#define luaP_setNetwork(lua_state, data) Util::luaP_registerstore(lua_state, "NETWORK", data)
#define luaP_getNetwork(lua_state) Util::luaP_registerget<Pro::Networking::Network>(lua_state, "NETWORK")

#define luaP_setEventHandler(lua_state, data) Util::luaP_registerstore(lua_state, "EVENT_HANDELER", data)
#define luaP_getEventHandler(lua_state) Util::luaP_registerget<Pro::EventHandler>(lua_state, "EVENT_HANDELER")

#define luaP_setSpriteManager(lua_state, data) Util::luaP_registerstore(lua_state, "SPRITE_MANAGER", data)
#define luaP_getSpriteManager(lua_state) Util::luaP_registerget<Pro::Graphics::SpriteManager>(lua_state, "SPRITE_MANAGER")

#define luaP_setTimer(lua_state, data) Util::luaP_registerstore(lua_state, "TIMER", data)
#define luaP_getTimer(lua_state) Util::luaP_registerget<Pro::Util::Timer>(lua_state, "TIMER")

	}
}
