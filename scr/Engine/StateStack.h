#pragma once

#include "Gamestate.h"
#include <vector>

namespace Pro{

	class StateStack{
		std::vector<GameState> m_stack;
		lua_State* lua_state;
	public:
		StateStack(){ lua_state = Util::luaP_state(nullptr); } 
		~StateStack(){ }
		// The bool if true will stop the state below (originally on top) it in the stack from updating
		// if false then it will also recieve updates.
		// This allows us to not render the menu when we push on our game state,
		// but all us to push ontop of that game state a GUI without stopping the game state.
		void push(GameState& state, bool deactivateTop){
			if (deactivateTop)
				m_stack.back().deactivate();
			state.activate();
			state.initialize(lua_state);
			m_stack.push_back(state);
		}

		// The stack that is poped will have it's cleanup function called
		// if one has been defined.
		void pop(){
			// Don't pop the base stack
			if (m_stack.size() == 1)
				return;
			m_stack.back().cleanup(lua_state);
			m_stack.pop_back();
			m_stack.back().returned(lua_state);
		}

		// executes the stack, starting at the base
		void execute(){
			// execute everything in the order the were placed
			// apart from the base stack, which the render function
			// is the last thing called of the frame 
			m_stack[0].update(lua_state);
			for (size_t x = 1; x < m_stack.size(); x++){
				// only execute the state if it's active
				auto& state = m_stack[x];
				if (state.isActive()){
					state.update(lua_state);
					state.render(lua_state);
				}
			}
			m_stack[0].render(lua_state);
		}

		static int lPop(lua_State* L){
			const auto stack = Util::luaP_touserdata<StateStack>(L, 1);
			stack->pop();
			return 0;
		}

		static int lPush(lua_State* L){ 
			const auto stack = Util::luaP_touserdata<StateStack>(L, 1);
			stack->push(*Util::luaP_touserdata<GameState>(L, 2), lua_toboolean(L, 3));
			return 0;
		}

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