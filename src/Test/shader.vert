#version 420

in vec3 position; 
in vec2 in_tex;
in vec3 in_normal;

out vec2 out_tex;
out vec3 out_normal; 
out vec3 color;

uniform mat4 view;
uniform mat4 model;
uniform mat4 scale;

uniform int has_normal;
uniform int has_tex_coord;

void main() { 
	color = vec3(1, 0, 0);
	if(has_normal == 1){ out_normal = in_normal; }
	if(has_tex_coord == 1){out_tex = in_tex; }
	gl_Position = view * model * vec4(position, 1);
}
