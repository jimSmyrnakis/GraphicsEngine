#version 430

layout( location = 0 ) out vec4 fragColor;

// material light colors 
uniform vec3 u_ambient; // basic color 
uniform vec3 u_diffuse; // 
uniform vec3 u_specular;
uniform float u_shininess;

in vec3 vNormal; // in view space 
in vec3 vPosition; // in view space 
in vec3 vLightPosition; // in view space 

void main()
{
	// view space normal 
	vec3 view_normal = normalize( vNormal );

	// view space direction 
	vec3 view_dir = normalize( -vPosition );

	// inverse normal direction if is more than 90 degrees different 
	// scene has these problem 
	float n_dot_v = dot( view_normal, view_dir );
	if ( n_dot_v < 0.f ){
		view_normal = -view_normal;
	}
		

	// light dir in view space 
	vec3 light_dir = normalize( vLightPosition - vPosition );

	// reflect direction of light based on normal in view space 
	vec3 reflect_dir = reflect( -light_dir , view_normal );
	
	// diffuse component
	float diffuse = max( dot( view_normal, light_dir ), 0.f );

	// specular component
	float cos_a = max( dot( reflect_dir , view_dir ), 0.f );
	float specular = pow( cos_a, u_shininess );

	// 

	fragColor = vec4( u_ambient.rgb + u_diffuse*diffuse + u_specular * specular  , 1.f );
}
