#pragma once

#include "shaders\shaders.h"
#include <BufferWriter.h>
#include <BufferReader.h>
#include "Shader.h"
#include <ArrayList.h>

namespace Pro {
	namespace Graphics { 
		 
		class ShaderFactory {  
			ArrayList<pair<string, GLenum>> uniforms;
			ArrayList<pair<string, GLenum>> attributes;

			ArrayList<Shader_Module> shader_segments;

		public:  
			/*! Shader's are appended in the order of function calls */

			void enable(Shader_Module module);

			/*! Generate Shader */
			Shader generate() const;
			 
			/*! Resets the instance to the default state */
			void reset(); 
		}; 

		/*!
			ShaderFactory fact; 
			fact.enable(Shader_Module::PHONG_LIGHTING);
			Shader shader = fact.generate();
			auto uniforms = fact.getUniforms();
			auto attributes = fact.getAttributes();

		
		*/
	}
}