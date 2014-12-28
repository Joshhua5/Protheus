#pragma once

#include <ArrayList.h>
#include <smart_pointer.h>
#include "Texture.h"

namespace Pro {
	namespace Graphics {
		using namespace Util;
		
		class TextureUnit {
			static ArrayList<unsigned> bound_textures;
			static GLint max_texture_units;
		public:
			TextureUnit() {
				glGetIntegerv(GL_MAX_TEXTURE_UNITS, &max_texture_units);
				bound_textures.resize(max_texture_units);
			}
				
			inline static bool bind(int texture_unit, const Texture* texture) {
				if (bound_textures.at(texture_unit) == texture->getTextureID() || texture_unit >= max_texture_units)
					return false;
				bound_textures[texture_unit] = texture->getTextureID(); 
				glActiveTexture(GL_TEXTURE0 + texture_unit);
				texture->bind();

				return true;
			}

			inline static bool bind(int texture_unit, const Texture& texture) {
				return bind(texture_unit, &texture);
			} 

			inline static bool bind(int texture_unit, const smart_pointer<Texture>& texture) {
				return bind(texture_unit, texture._ptr);
			}
		};
	}
}