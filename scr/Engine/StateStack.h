#pragma once

#include "Gamestate.h"
#include <vector>

namespace Pro{

	class StateStack{
		std::vector<GameState> m_stack;

	public:

		// The bool if true will stop the state below (originally on top) it in the stack from updating
		// if false then it will also recieve updates.
		// This allows us to not render the menu when we push on our game state,
		// but all us to push ontop of that game state a GUI without stopping the game state.
		void push(GameState, bool);

		// The stack that is poped will have it's cleanup function called
		// if one has been defined.
		void pop();

		// executes the stack, starting at the base
		void execute();
	};
}