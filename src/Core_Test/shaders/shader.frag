#version 420

//uniform vec3 inColor;

out vec4 vcolor;
in vec2 out_tex;
in vec3 out_normal; 
//in vec3 light_direction;
in vec3 color;

layout(binding = 0) uniform sampler2D tex;

uniform int has_normal;
uniform int has_tex_coord;

uniform vec3 world_pos;
uniform mat4 model;
uniform int light_point_count;
uniform vec3 light_ambient;
uniform int light_directional_count;
uniform float light_points[25];
uniform float light_directional[25];

void main() {
	vcolor = vec4(color, 1);
	
	if(has_tex_coord == 1 && has_normal == 1){
		//vcolor = vec4(out_normal, 1);//texture(tex, out_tex);
		vcolor = texture(tex, out_tex);// * vec4(out_normal, 1);  
		vec3 light_position = vec3(light_points[0], light_points[1], light_points[2]);
		vec4 light_color = vec4(light_points[3], light_points[4], light_points[5], 1);
		vec3 light_direction = normalize(light_position - world_pos);
		
		vcolor *= (light_color * dot(light_direction, normalize(out_normal))) + vec4(light_ambient, 1);
		
	}
	else if(has_tex_coord == 1)
		vcolor = texture(tex, out_tex);
	else if(has_normal == 1)
		vcolor = vec4(out_normal, 1); 
} 