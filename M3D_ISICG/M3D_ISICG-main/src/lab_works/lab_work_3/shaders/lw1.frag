#version 430
layout( location = 0 ) out vec4 frag_color;
in vec4 v_color;
void main() {

	frag_color = vec4(v_color.xyz  , 1);

}
