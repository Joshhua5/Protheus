/*************************************************************************
Protheus Source File.
Copyright (C), Protheus Studios, 2013-2014.
-------------------------------------------------------------------------

Description:

-------------------------------------------------------------------------
History:
- 20:05:2014: Waring J.
*************************************************************************/

#pragma once

#include <stack>
#include <Error.h> 
#include <BufferWriter.h>
#include <Vector4.h> 
#include "Sprite.h"
#include "Shader.h"

// Create a shader to take in position, dimensions and a texture
// use a 
namespace Pro{
	namespace Graphics {
		using namespace std;
		using namespace Util;
		using namespace Asset;
		using namespace Math;

		class SpriteBatcher
		{  
			SDL_Renderer* renderer; 
			
			CBuffer* verticies;
			BufferWriter* writer;

			static Shader vertex_shader;
			static Shader fragment_shader;
		public:
			SpriteBatcher(SDL_Renderer*);
			~SpriteBatcher();
			 
			void push(const Sprite&,
				Vector2& position,
				Vector2 dimensions,
				const  float scale = 1,
				const  float rotate = 0);

			void push(const Sprite&,
				const  Vector4& pos_dim,
				const  float scale = 1,
				const  float rotate = 0);

			void flush(); 
		};
	}
}
