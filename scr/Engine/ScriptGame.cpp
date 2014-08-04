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
	const auto text = luaP_getTextRenderer(lua_state);
	const auto renderer = luaP_getRenderer(lua_state);
	const auto spriteBatcher = renderer->getBatcher();
	const auto eventHandler = luaP_getEventHandler(lua_state);
	const auto timer = luaP_getTimer(lua_state);
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
	// Add the ScriptGame instance to Lua 
	luaP_newobject<ScriptGame>(lua_state, this);
	lua_setglobal(lua_state, "Game");

	luaP_newobject<SpriteBatcher>(lua_state, luaP_getRenderer(lua_state)->getBatcher());
	lua_setglobal(lua_state, "SpriteBatcher");

	luaP_newobject<Renderer>(lua_state, luaP_getRenderer(lua_state));
	lua_setglobal(lua_state, "Renderer");

	luaP_newobject<SpriteManager>(lua_state, luaP_getSpriteManager(lua_state));
	lua_setglobal(lua_state, "SpriteManager");

	luaP_newobject<EventHandler>(lua_state, luaP_getEventHandler(lua_state));
	lua_setglobal(lua_state, "EventHandler");

	luaP_newobject<Timer>(lua_state, luaP_getTimer(lua_state));
	lua_setglobal(lua_state, "Timer");

	luaP_newobject<Network>(lua_state, luaP_getNetwork(lua_state));
	lua_setglobal(lua_state, "Network");

	luaP_newobject<TextRenderer>(lua_state, luaP_getTextRenderer(lua_state));
	lua_setglobal(lua_state, "Text");


	// Place the base state on the stack

	GameState base_state;
	base_state.setInitialize("Initialize");
	base_state.setUpdate("Update");
	base_state.setRender("Render");
	base_state.setCleanup("Cleanup");
	base_state.setReturn("Return");
	stack.push(base_state, false);

	SDL_ShowWindow(luaP_getSDLWindow(lua_state));
	return 0;

} 