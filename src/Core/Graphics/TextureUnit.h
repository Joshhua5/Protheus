#pragma once

#include <ArrayList.h>
#include <smart_ptr.h>
#include "Texture.h"

namespace Pro {
	namespace Graphics {
		using namespace Util;
		
		class TextureUnit {
			static std::vector<unsigned> bound_textures;
			static GLint max_texture_units;
		public:
			TextureUnit() {
				glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_texture_units);
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

			inline static bool bind(int texture_unit, const smart_ptr<Texture>& texture) {
				return bind(texture_unit, texture.get());
			}

			inline static GLint max_textures(){
				return max_texture_units;
			}
		};
	}
}