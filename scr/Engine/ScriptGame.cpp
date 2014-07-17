#include "ScriptGame.h"

using namespace Pro;

ScriptGame::ScriptGame(lua_State* L){
	lua_state = L;
}

ScriptGame::ScriptGame(){
	lua_state = nullptr;
}

ScriptGame::~ScriptGame(){}

int ScriptGame::update(){
	lua_getglobal(lua_state, "Update");
	return lua_pcall(lua_state, 0, 0, 0);
}

int ScriptGame::gameLoop(){ 
	const auto renderer = luaP_getRenderer(lua_state);
	const auto spriteBatcher = renderer->getBatcher();
	const auto eventHandler = luaP_getEventHandler(lua_state);
	const auto timer = luaP_getTimer(lua_state);
	do{ 
		timer->tick();
		eventHandler->update();
		renderer->startFrame();
		Util::checkError(lua_state, update());
		Util::checkError(lua_state, render());
		spriteBatcher->flush();
		renderer->endFrame();
	} while (!exitRequested);
	return 0;
}

int ScriptGame::render(){
	lua_getglobal(lua_state, "Render");
	return lua_pcall(lua_state, 0, 0, 0); 
}
int ScriptGame::initialize(){
	// Add the ScriptGame instance to Lua 
    Util::luaP_newobject<ScriptGame>(lua_state, this);
	lua_setglobal(lua_state, "Game");

	Util::luaP_newobject<Graphics::SpriteBatcher>(lua_state, luaP_getRenderer(lua_state)->getBatcher());
	lua_setglobal(lua_state, "SpriteBatcher");

	Util::luaP_newobject<Graphics::Renderer>(lua_state, luaP_getRenderer(lua_state));
	lua_setglobal(lua_state, "Renderer");
	 
	Util::luaP_newobject<Graphics::SpriteManager>(lua_state, luaP_getSpriteManager(lua_state));
	lua_setglobal(lua_state, "SpriteManager");

	Util::luaP_newobject<EventHandler>(lua_state, luaP_getEventHandler(lua_state));
	lua_setglobal(lua_state, "EventHandler");

	Util::luaP_newobject<Util::Timer>(lua_state, luaP_getTimer(lua_state));
	lua_setglobal(lua_state, "Timer");

	Util::luaP_newobject<Networking::Network>(lua_state, luaP_getNetwork(lua_state));
	lua_setglobal(lua_state, "Network");
	
	// Call initialize in the lua script
	lua_getglobal(lua_state, "Initialize");
	Util::checkError(lua_state, lua_pcall(lua_state, 0, 0, 0));
	SDL_ShowWindow(luaP_getSDLWindow(lua_state));
	return 0;
}
int ScriptGame::cleanup(){
	// Engine managed cleanup
	return 0;
}