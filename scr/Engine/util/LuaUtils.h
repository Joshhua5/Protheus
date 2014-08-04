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
#include "Error.h"
#include <vector>
#include <iostream>
using namespace std;

namespace Pro{
	namespace Util{
		inline void checkError(lua_State* L,const int error_code){
			if (error_code)
				error.reportError("Lua Error:" + string(lua_tostring(L, -1))); 
		}

		template<typename T>
		struct luaP_array{
			vector<T> elements;
			void add(T& value){
				elements.push_back(value);
			}
		};

		inline lua_State* luaP_state(lua_State* L){
			static lua_State* state = L;
			return state;
		}

		// The first part of the pair will be used as the key and the second as the data
		template<typename T>
		inline void luaP_pushtoarray(lua_State* L, string table_name, char* key, T data){
			lua_pushstring(L, table_name.data());
			lua_gettable(L, -1);
			lua_pushstring(L, key);
			lua_pushnumber(L, data);
			lua_settable(L, -3);
		}

		// push an array of complex data types to lua
		template<typename T>
		inline void luaP_pusharray(lua_State* L, vector<pair<const string, T>>& elements){
			lua_createtable(L, 0, 0);
			for (size_t x = 0; x < elements.size(); ++x){
				lua_pushstring(L, elements.at(x).first.data());
				luaP_newobject(L, elements.at(X).second);
				lua_settable(L, -3);
			}
		}

		template<>
		inline void luaP_pusharray(lua_State* L, vector<pair<const string, double>>& elements){
			lua_createtable(L, 0, 0);
			for each(const auto element in elements){
				lua_pushstring(L, element.first.data());
				lua_pushnumber(L, element.second);
				lua_settable(L, -3);
			}
		}

		// Push an array of primative data types to lua
		template<typename T>
		inline void luaP_pusharray(lua_State* L, T* data, size_t size){
			lua_createtable(L, 0, 0);
			for (unsigned int x = 0; x < size; ++x){
				lua_pushnumber(L, x);
				lua_pushnumber(L, data[x]);
				lua_settable(L, -3);
			}
		}

		template<typename T> T* luaP_touserdata(lua_State* L, int idx){
#ifdef DEBUG
			if (lua_isuserdata(L, idx) == false){
				error.reportError("Incorrect parameter");
				luaP_dumpLuaStack(L);
				return nullptr;
			}
#endif 
			return *static_cast<T**>(lua_touserdata(L, idx));
		}

		inline void luaP_setmetatable(lua_State* L, const string& metatable){
			luaL_getmetatable(L, metatable.data());
			lua_setmetatable(L, -2);
		}

		inline void luaP_setmetatable(lua_State* L, const char* metatable){
			luaL_getmetatable(L, metatable);
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
#ifdef DEBUG 
			if (data == nullptr){
				error.reportError("Null data passed to new object");
				luaP_dumpLuaStack(L);
				return 0;
			} 
#endif 
			T** o = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
			*o = data;
			luaP_setmetatable(L, T::lGetMetatable());
			return o;
		}

#define luaP_pushuserdata(L, data) Util::luaP_newobject(L, data)

		template<typename T> T** luaP_newobject(lua_State* L, T& data) {
			T** o = static_cast<T**>(lua_newuserdata(L, sizeof(T*)));
			*o = new T(data);
			luaP_setmetatable(L, T::lGetMetatable());
			return o;
		}

		inline void luaP_registerstore(lua_State* L, const std::string& key, void* data){
			lua_pushstring(L, key.data());
			lua_pushlightuserdata(L, data);
			lua_settable(L, LUA_REGISTRYINDEX);
		}

		inline void luaP_checkerror(lua_State* L, int error_code){
			if (error_code != LUA_OK){
				cout << lua_tostring(L, -1) << endl;
			}
		}

		template<typename T> T* luaP_registerget(lua_State* L, const std::string& key){
			lua_pushstring(L, key.data());
			lua_gettable(L, LUA_REGISTRYINDEX);
			const auto pointer = static_cast<T*>(lua_touserdata(L, -1));
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

		// Returns a string as a pair, the second value contains the length
		inline string luaP_tostring(lua_State* L, int idx){ 
			return string(lua_tostring(L, idx));
		}

		/* Useful macro's*/

#define luaP_pushnumber(L, i) lua_pushnumber(L, static_cast<lua_Number>(i))

#define luaP_toint(L, i)		static_cast<int>(lua_tonumber(L, i))
#define luaP_touint(L, i)		static_cast<unsigned>(lua_tonumber(L, i))
#define luaP_togameid(L, i)		static_cast<game_id>(lua_tonumber(L, i))
#define luaP_toushort(L, i)		static_cast<unsigned short>(lua_tonumber(L, i))
#define luaP_tofloat(L, i)		static_cast<float>(lua_tonumber(L, i))
#define luaP_todouble(L, i)		static_cast<double>(lua_tonumber(L, i))  

#define	luaP_pushnumber(L, i)	lua_pushnumber(L, static_cast<lua_Number>(i))

#define luaP_getFileSystem(lua_state) Pro::Util::luaP_registerget<Pro::Util::FileSystem>(lua_state, "FILESYSTEM")
#define luaP_setFileSystem(lua_state, data) Pro::Util::luaP_registerstore(lua_state, "FILESYSTEM", data)

#define luaP_setTextRenderer(lua_state, data) Util::luaP_registerstore(lua_state, "TEXTRENDERER", data)
#define luaP_getTextRenderer(lua_state) Util::luaP_registerget<Pro::Graphics::TextRenderer>(lua_state, "TEXTRENDERER")

#define luaP_setWindow(lua_state, data) Util::luaP_registerstore(lua_state, "WINDOW", data)
#define luaP_getWindow(lua_state) Util::luaP_registerget<Pro::Window>(lua_state, "WINDOW")

#define luaP_setRenderer(lua_state, data) Util::luaP_registerstore(lua_state, "RENDERER", data)
#define luaP_getRenderer(lua_state) Util::luaP_registerget<Pro::Graphics::Renderer>(lua_state, "RENDERER")

#define luaP_setSDLRenderer(lua_state, data) Util::luaP_registerstore(lua_state, "SDL_RENDERER", data)
#define	luaP_getSDLRenderer(lua_state) Util::luaP_registerget<SDL_Renderer>(lua_state, "SDL_RENDERER")

#define luaP_setSDLWindow(lua_state, data) Util::luaP_registerstore(lua_state, "SDL_WINDOW", data)
#define	luaP_getSDLWindow(lua_state) Util::luaP_registerget<SDL_Window>(lua_state, "SDL_WINDOW")

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
