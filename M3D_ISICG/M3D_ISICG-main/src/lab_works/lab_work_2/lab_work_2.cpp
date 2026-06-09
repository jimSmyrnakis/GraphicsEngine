#include "lab_work_2.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork2::_shaderFolder = "src/lab_works/lab_work_2/shaders/";

	LabWork2::~LabWork2()
	{
		glDeleteShader( _program );
		glDeleteShader( vao );
		glDeleteBuffers(1, &vbo_positions );
		glDeleteBuffers( 1, &vbo_colors );
	}

	bool LabWork2::init()
	{
		
		mesh = { Vec2f( .5, .5 ), Vec2f( -.5, .5 ), Vec2f( .5, -.5 ), Vec2f( -.5, -.5 ) };
		colorBuffer = { Vec4f( 1, 0, 0, 1 ), Vec4f( 0, 1, 0, 1 ), Vec4f( 0, 0, 1, 1 ), Vec4f( 1, 1, 1, 1 ) };
		indexBuffer	= { 0, 1, 2, 1 , 2 ,3 };

		std::cout << "Initializing lab work 1..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		
		// Vertex Shader
		std::string vertexShaderStr = readFile( _shaderFolder + "lw1.vert" );
		const char * vsrc			 = vertexShaderStr.c_str();
		GLuint			  vshader		  = glCreateShader( GL_VERTEX_SHADER );
		glShaderSource( vshader,
						1,
						&vsrc,
						NULL ); // vertex_shader_source is a GLchar* containing glsl shader source code
		glCompileShader( vshader );

		GLint vertex_compiled;
		glGetShaderiv( vshader, GL_COMPILE_STATUS, &vertex_compiled );
		if ( vertex_compiled != GL_TRUE )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( vshader, sizeof( log ), NULL, log );
			glDeleteShader( vshader );
			std ::cerr << " Error compiling vertex shader : " << log << std ::endl;
			return false;

		}


		// Fragment Shader
		std::string fragmentShaderStr = readFile( _shaderFolder + "lw1.frag" );
		const char * fsrc			   = fragmentShaderStr.c_str();
		GLuint fshader = glCreateShader( GL_FRAGMENT_SHADER );
		glShaderSource( fshader,
						1,
						&fsrc,
						NULL ); // fragment_shader_source is a GLchar* containing glsl shader source code
		glCompileShader( fshader );

		GLint fragment_compiled;
		glGetShaderiv( fshader, GL_COMPILE_STATUS, &fragment_compiled );
		if ( fragment_compiled != GL_TRUE )
		{
			GLchar log[ 1024 ];
			glGetShaderInfoLog( fshader, sizeof( log ), NULL, log );
			glDeleteShader( fshader );
			glDeleteShader( vshader );
			std ::cerr << " Error compiling fragment shader : " << log << std ::endl;
			return false;
		}

		// Final program linking
		_program = glCreateProgram();
		glAttachShader( _program, vshader );
		glAttachShader( _program, fshader );
		glLinkProgram( _program );

		GLint program_linked;
		glGetProgramiv( _program, GL_LINK_STATUS, &program_linked );
		if ( program_linked != GL_TRUE )
		{
			GLchar log[ 1024 ];
			glGetProgramInfoLog( _program, sizeof( log ), NULL, log );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}
		std::cout << "Compiling Done!" << std::endl;

		// Mesh data
		glCreateBuffers( 1, &vbo_positions );
		glNamedBufferData( vbo_positions, mesh.size() * sizeof( Vec2f ), mesh.data(), GL_STATIC_DRAW );

		glCreateBuffers( 1, &ebo );
		glNamedBufferData( ebo, indexBuffer.size() * sizeof( int ), indexBuffer.data(), GL_STATIC_DRAW );

		glCreateBuffers( 1, &vbo_colors );
		glNamedBufferData( vbo_colors, colorBuffer.size() * sizeof( Vec4f ), colorBuffer.data(), GL_STATIC_DRAW );


		glCreateVertexArrays( 1, &vao );
		glEnableVertexArrayAttrib( vao, 0 );
		glVertexArrayAttribFormat( vao, 0, 2, GL_FLOAT, GL_FALSE , 0 );
		glVertexArrayVertexBuffer( vao, 0, vbo_positions, 0, sizeof( Vec2f ) );
		glVertexArrayAttribBinding( vao, 0, 0 );

		glEnableVertexArrayAttrib( vao, 1 );
		glVertexArrayAttribFormat( vao, 1, 4, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( vao, 1, vbo_colors, 0, sizeof( Vec4f ) );
		glVertexArrayAttribBinding( vao, 1, 1 );

		glVertexArrayElementBuffer( vao, ebo );

		uTranslationX_location = glGetUniformLocation( _program, "uTranslationX" );
		u_brightness_location  = glGetUniformLocation( _program, "u_brightness" );
		
		
		return true;
	}

	void LabWork2::animate( const float p_deltaTime ) { 
		_time += p_deltaTime;
		uTranslationX_variable = glm::sin( _time ) / 2.0f;
	}

	void LabWork2::render()
	{
		
		 glClear( GL_COLOR_BUFFER_BIT );
		
		 glBindVertexArray( vao );
		glProgramUniform1f( _program, uTranslationX_location, uTranslationX_variable );
		 glUseProgram( _program );
		 glDrawElements( GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, nullptr );
		 glBindVertexArray( 0 );

	}

	void LabWork2::handleEvents( const SDL_Event & p_event )
	{}

	void LabWork2::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();

		bool Update_var =  ImGui::SliderFloat( "Brighness", &_Brightness, 0, 1, nullptr, 0 );
		if (Update_var) {
			glProgramUniform1f( _program, u_brightness_location, _Brightness );
		}
		float color[ 3 ];
		Update_var = ImGui::ColorEdit3( "BackGround", glm::value_ptr( _bgColor ), 0 );
		if (Update_var) {
			glClearColor( _bgColor.r, _bgColor.g, _bgColor.b, 1 );
		}
	}

} // namespace M3D_ISICG
