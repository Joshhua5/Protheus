#pragma once

#include "StateStack.h" 

using namespace Pro;
using namespace std;

StateStack::StateStack(){ lua_state = Util::luaP_state(nullptr); }
StateStack::~StateStack(){ }

void StateStack::push(GameState& state, bool deactivateTop){
	// Deactivate the current top state, if true
	if (deactivateTop)
		m_stack.back().deactivate();
	// Make sure the new state is active
	state.activate();
	// Add the new state to the stack
	m_stack.push_back(state);
	// Call the state's initialize function
	state.initialize(lua_state);
}

void StateStack::pushBase(const GameState& state){
	// Move the base state, as we don't need a copy
	base_state = std::move(state);
	base_state.initialize(lua_state);
}

// The stack that is poped will have it's cleanup function called
// if one has been defined.
void StateStack::pop(){
	switch (m_stack.size()){
	case 0:
		return; // don't pop if empty
	case 1:
		m_stack.back().cleanup(lua_state);
		m_stack.pop_back();
		return; // don't call return if there's nothing returned too
	default:
		m_stack.back().cleanup(lua_state);
		m_stack.pop_back();
		m_stack.back().returned(lua_state);
		return;
	} 
}
 
void StateStack::execute(){
	// execute everything in the order the were placed
	// apart from the base stack, which has it's update function called first
	// and render function called last.
	base_state.update(lua_state);
	for each(auto state in m_stack)
		// Ignore a state that isn't active
		if (state.isActive()){
			state.update(lua_state);
			state.render(lua_state);
		}
	base_state.render(lua_state);
}

int StateStack::lPop(lua_State* L){
	// Static because there's only one instance of the StateStack.
	static const auto stack = Util::luaP_touserdata<StateStack>(L, 1);
	stack->pop();
	return 0;
}

int StateStack::lPush(lua_State* L){
	static const auto stack = Util::luaP_touserdata<StateStack>(L, 1);
	switch (lua_gettop(L)){
	case 2:
		stack->push(*Util::luaP_touserdata<GameState>(L, 2), false);
		return 0;
	case 3:
		stack->push(*Util::luaP_touserdata<GameState>(L, 2), luaP_toboolean(L, 3));
		return 0;
	} 
	return 0;
}