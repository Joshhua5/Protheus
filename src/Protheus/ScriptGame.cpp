#include "ScriptGame.h"

using namespace Pro;
using namespace Util;
using namespace Graphics;
using namespace Networking;

ScriptGame::ScriptGame(lua_State* L){
	lua_state = L;
}

ScriptGame::ScriptGame(){
	lua_state = nullptr;
}

ScriptGame::~ScriptGame(){}

int ScriptGame::gameLoop(){
	// Static because they only have one instance.
	static const auto text = luaP_getTextRenderer(lua_state);
	static const auto renderer = luaP_getRenderer(lua_state);
	static const auto spriteBatcher = renderer->getBatcher();
	static const auto eventHandler = luaP_getEventHandler(lua_state);
	static const auto timer = luaP_getTimer(lua_state);
	do{
		// Update engine
		timer->tick();
		eventHandler->update();
		renderer->startFrame();
		// Update Stack
		stack.execute();
		// Render
		spriteBatcher->flush();
		text->flush();
		renderer->endFrame();
	} while (!exitRequested);
	return 0;
}

int ScriptGame::initialize(){
	// Initialize the base state for Protheus
	// running in script mode.

	GameState base_state;
	base_state.setInitialize("Initialize");
	base_state.setUpdate("Update");
	base_state.setRender("Render");
	base_state.setCleanup("Cleanup");
	base_state.setReturn("Return");
	// 
	stack.pushBase(std::move(base_state));

	SDL_ShowWindow(luaP_getSDLWindow(lua_state));
	return 0;
} 