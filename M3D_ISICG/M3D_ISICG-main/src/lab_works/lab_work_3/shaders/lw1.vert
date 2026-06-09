#version 450
layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

out vec4 v_color;

uniform mat4 u_mvp;
void main() {
	
	gl_Position = position * u_mvp;
	v_color = color;
}

