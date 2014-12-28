#version 400

struct Point_Light{
	vec3 position;
	vec3 color;
	float intensity;
	float attenuation;
	float diffuse;
	float specular;
}

struct Directional_Light{
	vec3 position;
	vec3 color;
	vec3 direction;
	float intensity;
	float attenuation;
}