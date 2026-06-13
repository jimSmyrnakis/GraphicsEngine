#version 430

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;

uniform mat4 uMVPMatrix; // Projection * View * Model
uniform mat4 uMVMatrix; // View * Model
uniform vec3 u_lightPosition;

out vec3 vNormal;
out vec3 vPosition;
out vec3 vLightPosition;
void main()
{

	mat3 normalMatrix = mat3(transpose(inverse(uMVMatrix)));

	// normal in view space 
	vNormal = normalize(normalMatrix * aVertexNormal).xyz;
	
	// light position in view space 
	vLightPosition = vec3(uMVMatrix * vec4(u_lightPosition, 1.f));
	
	// vertex position in view space 
	vPosition = vec3(uMVMatrix * vec4(aVertexPosition, 1.f));

	// vertex position in clip space 
	gl_Position = uMVPMatrix * vec4( aVertexPosition, 1.f );
}
