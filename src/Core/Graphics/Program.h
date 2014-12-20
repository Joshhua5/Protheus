#pragma once

#include <Vector3.h>
#include <Matrix44.h>
#include <mutex>
#include <unordered_map>
#include "extern\glew.h"
#include "Shader.h"

namespace Pro {
	class Program {
		GLuint program_id;
		bool has_error = false; 
		std::unordered_map<std::string, GLint> locations;
		//std::mutex lock; 

		GLint getUniformLocation(GLuint program_id, const string& uniform_name);

		// non copyable
		Program(const Program&) = delete;
		Program& operator=(const Program&) = delete;
	public:
		// if false is passed, init() must be called before using
		// this instance
		Program(bool initialize_gl = true);
		~Program();
		Program(Program&&);
		Program& operator=(Program&&);

		void attachShader(const Shader& shader);

		void init();

		void link();

		bool hasError() const;
		  
		// Binds the program and stores the previously 
		// program in use
		void preservedUse();
		void use();
		// returns the used program to the original value
		// which was set at the previous preservedUse call
		void preservedDisuse();


		GLuint getID() const;

		// If the stride is equal to 0 then the function call is silently returned and no state is changed
		 void setUniform(const string& uniform_name, const Vector3<float>& value);
		void setUniform(const string& uniform_name, GLint value);
		void setUniform(const string& uniform_name, const Matrix44<float>& value); 
		void setUniform(const string& uniform_name, const GLint* value, unsigned size);
		void setUniform(const string& uniform_name, float* value, unsigned count);

		// If the stride is equal to 0 then the function call is silently returned and no state is changed

		// Program will be set active and then the state will be returned to previous bound program after,
		// For performance make sure that the program is already bound when setting attributes 
	};	
}