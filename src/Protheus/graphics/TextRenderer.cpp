#include "TextRenderer.h"

using namespace Pro;
using namespace Graphics;
using namespace Math;
using namespace Util;

TextRenderer::TextRenderer(lua_State* lua_state, SDL_Renderer* sdl_renderer)
{
	this->lua_state = lua_state;
	if (TTF_WasInit())
		TTF_Init();  

	renderer = sdl_renderer;
	active_font = nullptr;
}

TextRenderer::~TextRenderer(){
	TTF_Quit();
}

void TextRenderer::loadFont(const string& name, const string& path){
	auto font = TTF_OpenFont(path.data(), 72);
	// check for error
	if (font == nullptr){
		error.reportError("Unable to load font: \n" + path + "\n" + TTF_GetError());
		return;
	}
	// Set font as active is it's the first loaded
	if (active_font == nullptr)
		active_font = font;

	loaded_fonts.insert({ name, font }); 
}
  

void TextRenderer::pushText(const string& text, const Vector2& position, int fontSize = 10, float rotation = 0){ 
	Details details;
	details.size = (float)fontSize;
	details.position = position;
	details.color = Vector4(255, 255, 255, 0);
	details.text = text;
	details.rotation = rotation;
	text_stack.push(details);
}

void TextRenderer::flush(){
	if (active_font == nullptr) 
		return; 
	while (!text_stack.empty()){
		// Horribly inefficient, add method for reusing a texture,
		// instead of destroying and creating a texture every frame.
		auto text = text_stack.top(); 
		SDL_Color color = SDL_ColorCreate(text.color);
		auto surface = TTF_RenderText_Solid(active_font, text.text.data(), color);  
		auto texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
		SDL_Point dim;
		SDL_QueryTexture(texture, NULL, NULL, &dim.x, &dim.y);
		auto rect = SDLP_RectCreate(text.position.x, text.position.y, dim.x / text.size, dim.y / text.size);
		SDL_RenderCopyEx(renderer, texture, NULL, &rect, text.rotation, NULL, SDL_FLIP_NONE);
		SDL_DestroyTexture(texture);
	}
}

int TextRenderer::lPushText(lua_State* L){
	static const auto tr = luaP_touserdata<TextRenderer>(L, 1); 
	const auto str = lua_tostring(L, 2);
	const auto pos = *luaP_touserdata<Vector2>(L, 3);

	// remove one because the TextRenderer is passed by default
	const auto argument_count = lua_gettop(L);

	switch (argument_count){
	case 3:
		tr->pushText(str, pos);
		break;
	case 4:
		tr->pushText(str, pos, luaP_toint(L, 4));
		break;
	case 5:
		tr->pushText(str, pos, luaP_toint(L, 4), luaP_tofloat(L, 5));
		break; 
	default:
		error.reportMessage("Invalid number of arguments passed to pushText");
	}
	return 0;
}

int TextRenderer::lFlush(lua_State* L){
	static const auto tr = luaP_touserdata<TextRenderer>(L, 1);
	tr->flush();
	return 0;
}

int TextRenderer::lLoadFont(lua_State* L){
	static const auto tr = luaP_touserdata<TextRenderer>(L, 1);
	tr->loadFont(lua_tostring(L, 2), lua_tostring(L, 3));
	return 0;
}