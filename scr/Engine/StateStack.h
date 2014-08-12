#pragma once

#include "Gamestate.h"
#include <vector>

namespace Pro{
	using namespace std;

	class StateStack{
		vector<GameState> m_stack;
		GameState base_state; // the active state of a base_state is ignored.
		lua_State* lua_state;
	public:
		StateStack();
		~StateStack();

		// The bool if true will stop the state below (originally on top) it in the stack from updating
		// if false then it will also recieve updates.
		// This allows us to not render the menu when we push on our game state,
		// but all us to push ontop of that game state a GUI without stopping the game state.
		void push(GameState& state, bool deactivateTop);

		// accepts a GameState which will be used at the stack's base.
		// the base will remain active until Protheus exits.
		void pushBase(const GameState& state);

		// The stack that is poped will have it's cleanup function called
		// if one has been defined.
		void pop();

		// Executes the stack.
		void execute();

		static int lPop(lua_State* L);
		static int lPush(lua_State* L);

		// returns the Metatable's name assosiated with this object
		/*constexpr*/ static const char* lGetMetatable(){
			return "script_game_metatable";
		}

		template<typename T>
		static inline void lGetFunctions(std::vector<luaL_Reg>& fields){
			fields.push_back({ "push", &T::lPush });
			fields.push_back({ "pop", &T::lPop });
		}
	};
}