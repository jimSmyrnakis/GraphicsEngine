#version 430

layout( location = 0 ) in vec3 aVertexPosition;
layout( location = 1 ) in vec3 aVertexNormal;
layout( location = 2 ) in vec2 aVertexTexCoords;
layout( location = 3 ) in vec3 aVertexTangent;
layout( location = 4 ) in vec3 aVertexBitagent;

uniform mat4 u_MVPMatrix; // Projection * View * Model
uniform mat4 u_MVMatrix; // View * Model
uniform mat4 u_VMatrix; // View 
uniform mat4 u_TIVMatrix;
uniform vec3 u_lightPosition;

out vec3 v_Normal;
out vec3 v_Position;
out vec3 v_LightPosition;
out vec2 v_textCoords;
void main()
{

	//mat3 normalMatrix = mat3(transpose(inverse(u_MVMatrix)));
	mat3 normalMatrix = mat3(u_TIVMatrix);
	// normal in view space 
	v_Normal = normalize(normalMatrix * aVertexNormal).xyz;
	
	// light position in view space 
	v_LightPosition = vec3(u_VMatrix * vec4(u_lightPosition, 1.f));
	
	// vertex position in view space 
	v_Position = vec3(u_VMatrix * vec4(aVertexPosition, 1.f));

	v_textCoords = aVertexTexCoords;

	// vertex position in clip space 
	gl_Position = u_MVPMatrix * vec4( aVertexPosition, 1.f );

}
