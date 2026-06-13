#version 430

layout( location = 0 ) out vec4 fragColor;

// material base colors 
uniform vec3 u_ambient; // basic color 
uniform vec3 u_diffuse; // 
uniform vec3 u_specular;
uniform float u_shininess;

// material textures 
uniform bool u_hasAmbientMap;
uniform sampler2D u_ambientMap;
uniform bool u_hasDiffuseMap;
uniform sampler2D u_diffuseMap;
uniform bool u_hasSpecularMap;
uniform sampler2D u_specularMap;
uniform bool u_hasShininessMap;
uniform sampler2D u_shininessMap;


// varing variables 
in vec3 v_Normal; // in view space 
in vec3 v_Position; // in view space 
in vec3 v_LightPosition; // in view space 
in vec2 v_textCoords; // uvs

void main()
{
	vec3 ambient = u_ambient;
	if (u_hasAmbientMap) {
		ambient *= texture(u_ambientMap , v_textCoords).rgb;
	}
	// view space normal 
	vec3 view_normal = normalize( v_Normal );
	// view space direction 
	vec3 view_dir = normalize( -v_Position );

	// inverse normal direction if is more than 90 degrees different 
	// scene has these problem 
	float n_dot_v = dot( view_normal, view_dir );
	if ( n_dot_v < 0.f ){
		view_normal = -view_normal;
	}
	// light dir in view space 
	vec3 light_dir = normalize( v_LightPosition - v_Position );
	// reflect direction of light based on normal in view space 
	vec3 reflect_dir = reflect( -light_dir , view_normal );
	

	//shininess 
	float shininess = u_shininess;
	if (u_hasShininessMap){
		shininess += texture(u_shininessMap , v_textCoords).x;
	}

	// diffuse component
	float diffuse = max( dot( view_normal, light_dir ), 0.f );
	vec3 albedo = u_diffuse;
	if (u_hasDiffuseMap)
		albedo += texture(u_diffuseMap, v_textCoords).rgb;
	vec3 diffuseColor = albedo * diffuse;
	// specular component
	float cos_a = max( dot( reflect_dir , view_dir ), 0.f );
	float specular = pow( cos_a, u_shininess );
	vec3 specularIntensity = u_specular;
	if (u_hasSpecularMap){
		specularIntensity += texture(u_specularMap , v_textCoords).xxx;
	}
	vec3 specularColor = specularIntensity * specular;
	// 

	fragColor = vec4( ambient.rgb + diffuseColor + specularColor  , 1.f );
}
