#pragma once


namespace Pro {
	 
	class Model { 
		Model(const Model&) = delete;
		Model operator=(const Model&) = delete; 
	
		GLuint vba;
		GLuint vbo;
	public:
		Model(GLuint vbo, GLuint vba);
		Model(Model&&);
		Model operator=(Model&&);
		~Model(); 
	};
}