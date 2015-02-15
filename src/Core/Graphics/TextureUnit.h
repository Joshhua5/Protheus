#pragma once

#include <ArrayList.h>
#include <smart_ptr.h>
#include "Texture.h"

namespace Pro {
	namespace Graphics {
		using namespace Util;
		
		class TextureUnit {
			static Util::ArrayList<unsigned> bound_textures;
			static GLint max_texture_units;
		public:
			TextureUnit() {
				glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_texture_units);
				bound_textures.Resize(max_texture_units);
			}
				
			inline static bool Bind(int texture_unit, const Texture* texture) {
				if (bound_textures.At(texture_unit) == texture->textureID() || texture_unit >= max_texture_units)
					return false;
				bound_textures[texture_unit] = texture->textureID(); 
				glActiveTexture(GL_TEXTURE0 + texture_unit);
				texture->Bind();

				return true;
			}

			inline static bool Bind(int texture_unit, const Texture& texture) {
				return Bind(texture_unit, &texture);
			} 

			inline static bool Bind(int texture_unit, const smart_ptr<Texture>& texture) {
				return Bind(texture_unit, texture.get());
			}

			inline static GLint max_textures(){
				return max_texture_units;
			}
		};
	}
}