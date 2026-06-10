#include "lab_work_1.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include <iostream>

namespace M3D_ISICG
{
	const std::string LabWork1::_shaderFolder = "src/lab_works/lab_work_1/shaders/";

	LabWork1::~LabWork1() { 
		glDeleteShader( _program );
		glUseProgram( 0 );
		glBindVertexArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glDeleteBuffers( 1 , &vbo );
		glDeleteVertexArrays( 1, &vao );
	}

	bool LabWork1::init()
	{
		
		Vec2f dt;
		dt = {-.5 , 0.5 };
		mesh.push_back( dt );
		dt = { 0.5 , 0.5 };
		mesh.push_back( dt );
		dt = { 0.5 , -.5 };
		mesh.push_back( dt );

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
			glDeleteShader( fshader );
			glDeleteShader( vshader );
			std ::cerr << " Error linking program : " << log << std ::endl;
			return false;
		}
		std::cout << "Compiling Done!" << std::endl;

		// Mesh data
		
	#if OPENGL_VERSION_MAJOR == 4 && OPENGL_VERSION_MINOR >= 5
		glCreateBuffers( 1, &vbo );
		glNamedBufferData( vbo, mesh.size() * sizeof( Vec2f ), mesh.data(), GL_STATIC_DRAW );
		glCreateVertexArrays( 1, &vao );
		glEnableVertexArrayAttrib( vao, 0 );
		glVertexArrayAttribFormat( vao, 0, 2, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( vao, 0, vbo, 0, sizeof( Vec2f ) );
		glVertexArrayAttribBinding( vao, 0, 0 );
	#else
		glGenBuffers( 1, &vbo );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, mesh.size() * sizeof( Vec2f ), mesh.data(), GL_STATIC_DRAW );
		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );
		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, // attribute index
							   2, // vec2 = x, y
							   GL_FLOAT,
							   GL_FALSE,
							   sizeof( Vec2f ), // stride
							   (void *)0		// offset
		);
	#endif
		
		

		
		return true;
	}

	void LabWork1::animate( const float p_deltaTime ) {}

	void LabWork1::render() {
		
		 glClear( GL_COLOR_BUFFER_BIT );
		glUseProgram( _program );
		 glBindVertexArray( vao );

		 glDrawArrays( GL_TRIANGLES, 0, mesh.size()  );

		 glBindVertexArray( 0 );

	}

	void LabWork1::handleEvents( const SDL_Event & p_event )
	{}

	void LabWork1::displayUI()
	{
		ImGui::Begin( "Settings lab work 1" );
		ImGui::Text( "No setting available!" );
		ImGui::End();
	}

} // namespace M3D_ISICG
