#version 420

in vec3 position;
/*
in vec3 color;
in vec2 texcoord;

out vec2 tex_coord;
out vec3 out_color;
*/
uniform mat4 view;
uniform mat4 model;

void main() {
	//tex_coord = texcoord;
	//out_color = color;
	gl_Position = view * model * vec4(position, 1);
}
