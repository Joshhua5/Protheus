#version 410

in vec3 color;
out vec4 out_color;

void main() {
    //out_color = vec4(gl_FragDepth, gl_FragDepth,gl_FragDepth, 1);
	out_color = vec4(color, 0.5f);
}