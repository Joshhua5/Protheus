
in vec3 vertex;
in vec2 tex_coord
in unsigned char texture_id;

out vec3 out_vertex;
out vec2 out_tex_coord;
out unsigned char out_texture_id;

int main(){ 
	out_texture_id = texture_id;
	out_tex_coord = tex_coord;
	out_vertex = vertex;
}