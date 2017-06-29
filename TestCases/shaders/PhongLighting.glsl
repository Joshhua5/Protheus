#version 400 

vec4 final_color;
vec3 ambient_color;
vec3 model_position;
vec3 eye_position;
vec3 frag_normal;

void processLight(Point_Light light){
	const vec3 eye_direction = model_position - eye_position;
	const vec3 light_direction = model_position - light.position;

	#if HAS_MATERIALS
	float specular_light = material.specular * pow(dot(reflect(-light_direction, frag_normal), eye_direction), material.shininess);
	float diffuse_light * material.diffuse * max(0.0, cos(dot(frag_normal, light_direction))); 
	#elif
	float specular_light = dot(reflect(-light_direction,  frag_normal), eye_direction);
	float diffuse_light =  max(0.0, cos(dot(frag_normal, light_direction)));
	#endif 


	float specular = light.specular * specular_light;
	float diffuse = light.diffuse * diffuse_light; 
	final_color *= ambient_color + (specular + diffuse) * light.color;
}