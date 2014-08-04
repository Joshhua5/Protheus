#pragma once

#include <string>
#include "component/ActiveState.h"

using namespace std;
namespace Pro{
	class GameState_Function{
	public:
		char isLua;
		// returns void and has no arguments 
		void(*function_pointer)(); 
		string lua_function;

		GameState_Function(){
			function_pointer = nullptr;
			isLua = 2; // set to neither true of false 
		}
		void execute(lua_State* L){
			switch (isLua){
			case 1: // true
				lua_getglobal(L, lua_function.data());
				lua_pcall(L, 0, 0, 0); break;
			case 0: // false
				// Cast to a function pointer
				function_pointer(); break;
			}
		}
	};

	class GameState : public Component::ActiveState{
		GameState_Function m_update;
		GameState_Function m_render;
		GameState_Function m_initialize;
		GameState_Function m_return;
		GameState_Function m_cleanup;
	public:
		GameState(){}
		~GameState(){}

		// A function called 
		// when the stack is being updated
		void setUpdate(void(*cpp_function)());
		void setUpdate(const string& lua_function);
		void update(lua_State*);

		// A function called when
		// when the stack is being rendered
		void setRender(void(*cpp_function)());
		void setRender(const string& lua_function);
		void render(lua_State*);

		// A function called when
		// the state is first created
		void setInitialize(void(*cpp_function)());
		void setInitialize(const string& lua_function);
		void initialize(lua_State*);

		// A Function called when 
		// set to the top of the stack
		void setReturn(void(*cpp_function)());
		void setReturn(const string& lua_function);
		void returned(lua_State*);

		// A function called 
		// when removed from the stack
		void setCleanup(void(*cpp_function)());
		void setCleanup(const string& lua_function);
		void cleanup(lua_State*);

		static int lSetCleanup(lua_State*);
		static int lSetRender(lua_State*);
		static int lSetInitialize(lua_State*);
		static int lSetUpdate(lua_State*);
		static int lSetReturn(lua_State*);
		static int lCreate(lua_State*);

		// returns the Metatable's name assosiated with this object
		/*constexpr*/ static const char* lGetMetatable(){
			return "gamestate_metatable";
		}

		template<typename T>
		static void lGetFunctions(std::vector<luaL_Reg>& fields){
			fields.push_back({ "setCleanup", &T::lSetCleanup });
			fields.push_back({ "setRender", &T::lSetRender });
			fields.push_back({ "setInitialize", &T::lSetInitialize });
			fields.push_back({ "setUpdate", &T::lSetUpdate });
			fields.push_back({ "setReturn", &T::lSetReturn });
		}
	};
}

