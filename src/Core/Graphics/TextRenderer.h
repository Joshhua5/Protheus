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
#include <Vector4.h> 
#include <unordered_map>
#include <Error.h>
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
			 
			SDL_Renderer* renderer;
			stack<Details> text_stack;

			TTF_Font* active_font; 
			unordered_map<string, TTF_Font*> loaded_fonts;
		public: 
			TextRenderer(SDL_Renderer* renderer);
			~TextRenderer();
 
			void loadFont(const string& name, const string& path); 
			void pushText(const string& text, const Vector2& position, int fontSize, float rotation);
 
			void flush();
			  
		};
	}
}
