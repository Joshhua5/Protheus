#include "Gamestate.h"

using namespace Pro;
using namespace std;


void GameState::setUpdate(void(*cpp_function)()){
	m_update.isLua = false;
	m_update.function_pointer = cpp_function;
}
void GameState::setUpdate(const string& lua_function){
	m_update.isLua = true;
	m_update.lua_function = lua_function;
}
void GameState::update(lua_State* L){
	m_update.execute(L);
}

void GameState::setRender(void(*cpp_function)()){
	m_render.isLua = false;
	m_render.function_pointer = cpp_function;
}
void GameState::setRender(const string& lua_function){
	m_render.isLua = true;
	m_render.lua_function = lua_function;
}
void GameState::render(lua_State* L){
	m_render.execute(L);
}

void GameState::setInitialize(void(*cpp_function)()){
	m_initialize.isLua = false;
	m_initialize.function_pointer = cpp_function;
}
void GameState::setInitialize(const string& lua_function){
	m_initialize.isLua = true;
	m_initialize.lua_function = lua_function;
}
void GameState::initialize(lua_State* L){
	m_initialize.execute(L);
}

 
void GameState::setReturn(void(*cpp_function)()){
	m_return.isLua = false;
	m_return.function_pointer = cpp_function;
}
void GameState::setReturn(const string& lua_function){
	m_return.isLua = true;
	m_return.lua_function = lua_function;
}
void GameState::returned(lua_State* L){
	m_return.execute(L);
}
 
void GameState::setCleanup(void(*cpp_function)()){
	m_cleanup.isLua = false;
	m_cleanup.function_pointer = cpp_function;
}
void GameState::setCleanup(const string& lua_function){
	m_cleanup.isLua = true;
	m_cleanup.lua_function = lua_function;
}
void GameState::cleanup(lua_State* L){
	m_cleanup.execute(L);
}

int GameState::lSetCleanup(lua_State* L){
	const auto state = Util::luaP_touserdata<GameState>(L, 1);
	state->setCleanup(lua_tostring(L, 2));
	return 0;
}
int GameState::lSetRender(lua_State* L){
	const auto state = Util::luaP_touserdata<GameState>(L, 1);
	state->setRender(lua_tostring(L, 2));
	return 0;
}
int GameState::lSetInitialize(lua_State* L){
	const auto state = Util::luaP_touserdata<GameState>(L, 1);
	state->setInitialize(lua_tostring(L, 2));
	return 0;
}
int GameState::lSetUpdate(lua_State* L){
	const auto state = Util::luaP_touserdata<GameState>(L, 1);
	state->setUpdate(lua_tostring(L, 2));
	return 0;
}
int GameState::lSetReturn(lua_State* L){
	const auto state = Util::luaP_touserdata<GameState>(L, 1);
	state->setReturn(lua_tostring(L, 2));
	return 0;
}

int GameState::lCreate(lua_State* L){ 
	Util::luaP_newobject(L, new GameState()); 
	return 1;
}
