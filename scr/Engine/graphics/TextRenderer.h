/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 26:07:2014: Waring J.
*************************************************************************/

#pragma once

#include <stack> 
#include <SDL_ttf.h>

#include "..\math\Vector4.h"
#include "..\util\LuaUtils.h"
#include "Sprite.h"

namespace Pro{
	namespace Graphics {
		using namespace std; 
		using namespace Math;
		class TextRenderer
		{
			struct Details{
				float size;
				float rotation; 
				Vector2 position;
				Vector4 color;
				string text; 
			};

			lua_State* lua_state;
			SDL_Renderer* renderer;
			stack<Details> text_stack;

			TTF_Font* active_font; 
			unordered_map<string, TTF_Font*> loaded_fonts;
		public: 
			TextRenderer(lua_State* lua_state);
			~TextRenderer();
			
			void loadFont(const string& name, const string& path);
			void pushText(const string& text, const Vector2& position);
			void pushText(const string& text, const Vector2& position, int fontSize);
			void pushText(const string& text, const Vector2& position, int fontSize, float rotation);
			void flush();
			 
			static int lLoadFont(lua_State*);
			static int lPushText(lua_State*);
			static int lFlush(lua_State*);

			static inline string lGetMetatable(){
				return "text_renderer_metatable";
			}

			template<typename T>
			static void lGetFunctions(std::vector<luaL_Reg>& fields){
				fields.push_back({ "loadFont", &T::lLoadFont });
				fields.push_back({ "flush", &T::lFlush });
				fields.push_back({ "drawText", &T::lPushText });
			}
		};
	}
}
