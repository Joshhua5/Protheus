#version 410

in vec3 in_pos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 world;

out vec3 color;

void main(){ 
    // Could optimise this matrix multiplication to once on the cpu
    // instead of once per vertex on the gpu
    gl_Position = projection * world * model * vec4(in_pos, 1);
	color = in_pos;
}
