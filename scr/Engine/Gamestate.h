#pragma once

#include <string>
#include "component/ActiveState.h"

using namespace std;
namespace Pro{
	class GameState_Function{
	public:
		// if no function has been set, then isLua will equal 2
		char isLua; 
		// fucntion pointer that returns void and has no arguments 
		void(*function_pointer)(); 
		// name of the lua function to be called.
		string lua_function;

		GameState_Function(){
			function_pointer = nullptr;
			isLua = 2; // set to neither true of false 
		}
		void execute(lua_State* L){
			switch (isLua){
			case 1: // true
				lua_getglobal(L, lua_function.data());
				// No returns and no arguments.
				lua_pcall(L, 0, 0, 0); break;
			case 0: // false
				// Cast to a function pointer
				function_pointer(); break;
			}
		}
	};

	// Each state can accept two versions of a functions to call
	// but only one can be used at any one time.
	// The first type is a lua function with no arguments and no returns
	// The second type is a C++ function pointer with no arguments and a void return

	class GameState : public Component::ActiveState{
		GameState_Function m_update;
		GameState_Function m_render;
		GameState_Function m_initialize;
		GameState_Function m_return;
		GameState_Function m_cleanup;
	public:
		GameState(){}
		~GameState(){}

		// The update function is called when the stack
		// is being executed, refer to Gamestate.h for
		// the order in which the stack is executed.
		void setUpdate(void(*cpp_function)());
		void setUpdate(const string& lua_function);
		void update(lua_State*);

		// The render function is called when the stack
		// is being executed, refer to GameState.h for 
		// the order in which the stack is executed.
		void setRender(void(*cpp_function)());
		void setRender(const string& lua_function);
		void render(lua_State*);

		// This function is called when the state is
		// first stored on the stack, it should be used
		// for initialization of the state.
		void setInitialize(void(*cpp_function)());
		void setInitialize(const string& lua_function);
		void initialize(lua_State*);

		// The return function is called when
		// the top of the stack is set to this state
		// to inform the state that this is happened
		// Protheus will call it's return function.
		void setReturn(void(*cpp_function)());
		void setReturn(const string& lua_function);
		void returned(lua_State*);
		 
		// The clean up functions is called when the 
		// state is being removed from the stack, never
		// to be returned to again.
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

