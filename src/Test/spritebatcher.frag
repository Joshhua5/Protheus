 
out vec4 out_color;

in vec3 out_vertex;
in vec2 out_tex_coord;
in unsigned char out_texture_id;

uniform sampler2D samplers[18]

int main(){ 
	sampler = out_texture_id;
	out_color = texture(sampler, tex_coord); 
	out_vertex = vertex;
}