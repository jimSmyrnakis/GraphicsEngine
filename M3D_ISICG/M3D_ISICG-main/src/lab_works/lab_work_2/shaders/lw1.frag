#version 430
layout( location = 0 ) out vec4 frag_color;
in vec4 v_color;
uniform float u_brightness;
void main() {

	frag_color = vec4(v_color.xyz * u_brightness , 1);

}
