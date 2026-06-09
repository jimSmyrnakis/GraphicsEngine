#version 450
layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;

out vec4 v_color;

uniform float uTranslationX;
void main() {
	
	gl_Position = vec4(position.x + uTranslationX , position.y , 0 , 1);
	v_color = color;
}

