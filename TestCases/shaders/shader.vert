#version 420

in vec3 position; 
in vec2 in_tex;
in vec3 in_normal;

out vec2 out_tex;
out vec3 out_normal; 
//out vec3 light_direction;
out vec3 color;

uniform mat4 view;
uniform mat4 model;
uniform mat4 scale;
uniform mat4 projection;
uniform mat4 normal_matrix;

uniform int has_normal;
uniform int has_tex_coord;

uniform int light_point_count;
uniform vec3 light_ambient;
uniform int light_directional_count;
uniform float light_points[25];
uniform float light_directional[25];

void main() { 
	color = vec3(1, 0, 0);
	if(has_normal == 1){ 
		vec4 _out = normal_matrix * vec4(normalize(in_normal), 1);
		out_normal = _out.xyz; 
	}
	if(has_tex_coord == 1){out_tex = in_tex; }
	gl_Position = projection * view * model * vec4(position, 1);
}
