#version 420

//uniform vec3 inColor;

out vec4 vcolor;
in vec2 out_tex;
in vec3 out_normal; 
in vec3 color;

layout(binding = 0) uniform sampler2D tex;

uniform int has_normal;
uniform int has_tex_coord;

void main() {
	vcolor = vec4(color, 1);
	
	if(has_tex_coord == 1 && has_normal == 1)
		texture(tex, out_tex) * vec4(out_normal, 1);
	else if(has_tex_coord == 1)
		texture(tex, out_tex);
	else if(has_normal == 1)
		vec4(out_normal, 1); 
} 