#version 420

uniform vec3 inColor;

out vec4 vcolor;
in vec3 out_color;
in vec2 tex_coord;

layout(binding = 0) uniform sampler2D tex;

void main() {
  vcolor = texture(tex, tex_coord);//vec4(out_color, 1);
}
