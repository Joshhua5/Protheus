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
#include <smart_pointer.h>
#include "Program.h"
#include "Sprite.h"
#include "VertexArray.h"
#include "Shader.h" 
#include "Transformation.h"

// Create a shader to take in position, dimensions and a texture
// use a 
namespace Pro{
	namespace Graphics {
		using namespace std;

		class SpriteBatcher
		{    
			Buffer* verticies;
			BufferWriter* writer;
			Transformation batch_transformation;
			VertexArray vao;

			static GLint max_sprites;
			static GLint max_textures;
			unsigned current_sprite_count = 0;

			std::vector<smart_pointer<Texture>> textures;
			std::vector<unsigned> sprite_count;
			std::vector<std::vector<unsigned>> sprite_indicies;
			std::stack<unsigned short> free_textures;
			unsigned current_texture_count = 0;

			GLuint vertex_buffer_id;
			GLuint element_buffer_id;
			GLuint texture_uniforms; 

			static Shader vertex_shader;
			static Shader fragment_shader;
			static Shader geometry_shader;
			static Program batch_program;

			SpriteBatcher(const SpriteBatcher&) = delete;
			SpriteBatcher operator=(const SpriteBatcher&) = delete;

		public:
			SpriteBatcher(SpriteBatcher&&);
			SpriteBatcher&& operator=(SpriteBatcher&&);
			// Window must be created, before creating an instance of SpriteBatcher
			SpriteBatcher();
			~SpriteBatcher();
			 
			// Performance benifit to grouping textures together in push calls
			void push(int texture_id,
				Vector3<float> position,
				Vector2<float> dimensions,
				const  float scale = 1,
				const  float rotate = 0);


			// if -1, the maxiumum texture bindings has been hit.
			int attachTexture(smart_pointer<Texture> tex);

			void removeTexture(int);
			
			void setTransformation(const Transformation& trans);
			 
			void flush(); 

			void render();
			
			void reset();
		};
	}
}
