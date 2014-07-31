#pragma once

#include <string>
#include "component\ActiveState.h"

using namespace std;
namespace Pro{
	class GameState : Component::ActiveState{
		struct Function{
			bool isLua;
			union{
				// returns void and has no arguments
				void(*function_pointer)();
				char* lua_function;
			};
			Function(){
				function_pointer = nullptr;
				lua_function = nullptr;
			}
		};
		Function m_update;
		Function m_render;
		Function m_initialize;
		Function m_return;
		Function m_cleanup;
	public:

		// A function called 
		// when the stack is being updated
		void setUpdate(void* cpp_function);
		void setUpdate(const string& lua_function);
		void update();

		// A function called when
		// when the stack is being rendered
		void setRender(void* cpp_function);
		void setRender(const string& lua_function);
		void render();

		// A function called when
		// the state is first created
		void setInitialize(void* cpp_function);
		void setInitialize(const string& lua_function);
		void initialize();

		// A Function called when 
		// set to the top of the stack
		void setReturn(void* cpp_function);
		void setReturn(const string& lua_function);
		void returned();

		// A function called 
		// when removed from the stack
		void setCleanup(void* cpp_function);
		void setCleanup(const string& lua_function);
		void cleanup();


		// returns the Metatable's name assosiated with this object
		/*constexpr*/ static const char* lGetMetatable(){
			return "gamestate_metatable";
		}

		template<typename T>
		static void lGetFunctions(std::vector<luaL_Reg>& fields){
			//fields.push_back({ "push", &T::lPush });
		}
	};
}

