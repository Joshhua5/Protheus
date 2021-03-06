#pragma once
#include <string>

std::string vertex_shader = ""
"#version 410																 \n"
"																			 \n" 
"in vec3 in_normal;															 \n"
"in vec3 in_vertex;															 \n"
"																			 \n"
"uniform mat4 model;														 \n"
"uniform mat4 world;														 \n"
"uniform mat4 camera;														 \n"
"uniform mat4 projection;													 \n"
"uniform vec3 sun_vector;													 \n"
"																			 \n" 
"smooth out vec3 normal;													 \n"
"flat out vec3 sun;															 \n"
"																			 \n"
"void main() {																 \n"
"	gl_Position = projection * camera * world * model * vec4(in_vertex, 1);	 \n" 
"	sun = sun_vector;														 \n"
"	normal = in_normal;														 \n"
"}																			 \n"
;

std::string fragment_shader = ""
"#version 410																 \n"
"																			 \n"
"uniform vec3 ambient_color;												 \n"
"uniform vec3 diffuse_color;												 \n"
"																			 \n" 
"smooth in vec3 normal;														 \n"
"flat in vec3 sun;															 \n"
"																			 \n"
"out vec4 out_color;														 \n"
"																			 \n"
"void main() {																 \n"
"	vec3 diffuse_light = diffuse_color * max(0.0, cos(dot(normal, sun)));	 \n"
"   out_color = vec4(diffuse_light.r, diffuse_light.g, diffuse_light.b, 1);		 \n" 
"}";


std::string skybox_vert_shader = ""
"#version 410																 \n"
"																			 \n" 
"in vec2 in_texcoord;														 \n" 
"in vec3 in_vertex;															 \n" 
"																			 \n" 
"uniform mat4 camera;														 \n"
"uniform mat4 model;														 \n"
"uniform mat4 projection;													 \n" 
"																			 \n" 
"out vec2 texcoord;															 \n" 
"																			 \n"
"void main() {																 \n"
"	gl_Position = projection * camera * model * vec4(in_vertex, 1);			 \n"
"	texcoord = in_texcoord;													 \n" 
"}																			 \n"
;

std::string skybox_frag_shader = ""
"#version 410																 \n"
"																			 \n"
"uniform sampler2D tex;                                                      \n"
"																			 \n"
"in vec2 texcoord;															 \n" 
"																			 \n"
"out vec4 out_color;														 \n"
"																			 \n"
"void main() {																 \n" 
"	out_color = texture(tex, texcoord);									 	 \n"
"}";

