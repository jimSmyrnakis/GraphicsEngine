#version 430
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

out vec4 v_color;

uniform mat4 u_mvp;
void main() {
	
	gl_Position =   u_mvp * vec4(position.xyz , 1);
	v_color = color;
}

