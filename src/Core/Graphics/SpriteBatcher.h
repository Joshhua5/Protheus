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
#include <Log.h> 
#include <BufferWriter.h>
#include <initializer_list>
#include <Vector4.h> 
#include <smart_ptr.h>
#include <ArrayList.h>
#include "Program.h"
#include "Sprite.h"
#include "VertexArray.h"
#include "Shader.h" 
#include "TextureUnit.h"
#include "Transformation.h"
 
namespace Pro{
	namespace Graphics {
		using namespace std;

		class SpriteBatcher
		{    
			Buffer* verticies;
			BufferWriter* writer; 
			VertexArray vao;

			static GLint max_sprites;
			static GLint max_textures;
			std::atomic<unsigned> current_sprite_count = 0;

			Util::ArrayList<smart_ptr<Texture>> textures;
			Util::ArrayList<unsigned> sprite_count;
			// Quick swap 
			Util::ArrayList<Util::ArrayList<unsigned>> sprite_indicies;
			std::stack<unsigned short> free_textures;
			unsigned current_texture_count = 0;

			GLuint vertex_buffer_id;
			GLuint element_buffer_id;
			GLuint texture_uniforms; 

			static Shader vertex_shader;
			static Shader fragment_shader;
			static Shader geometry_shader;
			static Program batch_program;

			static std::mutex lk;

			SpriteBatcher(const SpriteBatcher&) = delete;
			SpriteBatcher operator=(const SpriteBatcher&) = delete;

		public:
			SpriteBatcher(SpriteBatcher&&);
			SpriteBatcher& operator=(SpriteBatcher&&);
			// Window must be created, before creating an instance of SpriteBatcher
			SpriteBatcher(const Vector2<float>& window_dimensions = { 100, 100 });
			~SpriteBatcher();
			 
			// Performance benifit to grouping textures together in push calls
			void Push(int texture_id,
				Vector3<float> position,
				Vector2<float> dimensions,
				const  float scale = 1,
				const  float rotate = 0);

			
			//void BatchPush(int texture,
			//	Vector3<float> position,
			//	Vector2<float> dimensions,
			//	const  float scale = 1,
			//	const  float rotate = 0);
			
			//void BatchUpdate(int texture, unsigned count);

			// if -1, the maxiumum texture bindings has been hit.
			int AttachTexture(smart_ptr<Texture> tex);
			/*! indicies will be populated with the index of the texture inside the sprite batcher */
			int AttachTexture(ArrayList<int>& indicies, const ArrayList<smart_ptr<Texture>>& texs);

			void RemoveTexture(int);
			  
			void Flush(); 
			 
			void Alpha(const Vector3<float>& color);
			void SetCameraDimensions(const Vector2<float>& position);
			void SetCameraPosition(const Vector3<float>& position);

			void Render();
			
			void Reset();
		};
	}
}
