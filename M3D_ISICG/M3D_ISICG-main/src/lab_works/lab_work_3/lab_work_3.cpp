#include "lab_work_3.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

namespace M3D_ISICG
{
	void Mesh::_createCube( GLuint program ) {
		// Mesh data
		positions = {
			Vec3f( -.5, -.5, -.5 ), // 0
			Vec3f( .5, -.5, -.5 ),	 // 1
			Vec3f( .5, .5, -.5 ),	 // 2
			Vec3f( -.5, .5, -.5 ),	 // 3

			Vec3f( -.5, -.5, .5 ), // 4
			Vec3f( .5, -.5, .5 ),	// 5
			Vec3f( .5, .5, .5 ),	// 6
			Vec3f( -.5, .5, .5 )	// 7
		};

		indices = { // Front (-Z)
					0,
					1,
					2,
					0,
					2,
					3,

					// Back (+Z)
					4,
					6,
					5,
					4,
					7,
					6,

					// Left (-X)
					4,
					0,
					3,
					4,
					3,
					7,

					// Right (+X)
					1,
					5,
					6,
					1,
					6,
					2,

					// Bottom (-Y)
					4,
					5,
					1,
					4,
					1,
					0,

					// Top (+Y)
					3,
					2,
					6,
					3,
					6,
					7
		};

		colors = {
			Vec3f( 1, 0, 0 ), // 0 red
			Vec3f( 0, 1, 0 ), // 1 green
			Vec3f( 0, 0, 1 ), // 2 blue
			Vec3f( 1, 1, 0 ), // 3 yellow

			Vec3f( 1, 0, 1 ), // 4 magenta
			Vec3f( 0, 1, 1 ), // 5 cyan
			Vec3f( 1, 1, 1 ), // 6 white
			Vec3f( 0, 0, 0 )  // 7 black
		};

	#if OPENGL_VERSION_MAJOR == 4 && OPENGL_VERSION_MINOR >= 5
		glCreateBuffers( 1, &vbo_positions );
		glNamedBufferData( vbo_positions, positions.size() * sizeof( Vec3f ), positions.data(), GL_STATIC_DRAW );

		glCreateBuffers( 1, &ebo );
		glNamedBufferData( ebo, indices.size() * sizeof( int ), indices.data(), GL_STATIC_DRAW );

		glCreateBuffers( 1, &vbo_colors );
		glNamedBufferData( vbo_colors, colors.size() * sizeof( Vec3f ), colors.data(), GL_STATIC_DRAW );

		glCreateVertexArrays( 1, &vao );
		glEnableVertexArrayAttrib( vao, 0 );
		glVertexArrayAttribFormat( vao, 0, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( vao, 0, vbo_positions, 0, sizeof( Vec3f ) );
		glVertexArrayAttribBinding( vao, 0, 0 );

		glEnableVertexArrayAttrib( vao, 1 );
		glVertexArrayAttribFormat( vao, 1, 3, GL_FLOAT, GL_FALSE, 0 );
		glVertexArrayVertexBuffer( vao, 1, vbo_colors, 0, sizeof( Vec3f ) );
		glVertexArrayAttribBinding( vao, 1, 1 );

		glVertexArrayElementBuffer( vao, ebo );
	#else
		glGenBuffers( 1, &vbo_positions );
		glBindBuffer( GL_ARRAY_BUFFER, vbo_positions );
		glBufferData( GL_ARRAY_BUFFER, positions.size() * sizeof( Vec3f ), positions.data(), GL_STATIC_DRAW );

		glGenBuffers( 1, &vbo_colors );
		glBindBuffer( GL_ARRAY_BUFFER, vbo_colors );
		glBufferData( GL_ARRAY_BUFFER, colors.size() * sizeof( Vec3f ), colors.data(), GL_STATIC_DRAW );

		glGenVertexArrays( 1, &vao );
		glBindVertexArray( vao );

		glGenBuffers( 1, &ebo );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( int ), indices.data(), GL_STATIC_DRAW );

		glEnableVertexAttribArray( 0 );
		glBindBuffer( GL_ARRAY_BUFFER, vbo_positions );
		glVertexAttribPointer( 0, // attribute index
							   3, // vec3 = x, y , z
							   GL_FLOAT,
							   GL_FALSE,
							   sizeof( Vec3f ), // stride
							   (void *)0		// offset
		);

		glEnableVertexAttribArray( 1 );
		glBindBuffer( GL_ARRAY_BUFFER, vbo_colors );
		glVertexAttribPointer( 1, // attribute index
							   3, // vec3 = rgb
							   GL_FLOAT,
							   GL_FALSE,
							   sizeof( Vec3f ), // stride
							   (void *)0		// offset
		);
	#endif

		// make transform matrix unit one
		u_model = glm::mat4( 1.f ); // yeaaaaah
		
		
	}

	const std::string LabWork3::_shaderFolder = "src/lab_works/lab_work_3/shaders/";
	 
	LabWork3::~LabWork3()
	{
		glDeleteProgram( _program );
	}

	bool LabWork3::init()
	{
		_camera.setFovy( 65 );
		_camera.setScreenSize( _windowWidth, _windowHeight );
		_camera.setPosition( glm::vec3( 0.f, 0.f, 3.f ) );
		_updateViewMatrix();

		std::cout << "Initializing lab work 2..." << std::endl;
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

		

		
		_cube._createCube( _program );

		u_mvp_location = glGetUniformLocation( _program, "u_mvp" );
		
		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LESS );
		return true;
	}
	float rot_angle = 0;
	void LabWork3::animate( const float p_deltaTime )
	{ 
		rot_angle += 45 * p_deltaTime; // rotate 45 degrees per second
		_cube.u_model = glm::mat4( 1.0f );

		// Translation
		_cube.u_model = glm::translate( _cube.u_model, glm::vec3( 0.0f, 0.0f, 0.0f ) );

		// Rotation
		// around y axis
		_cube.u_model = glm::rotate( _cube.u_model, glm::radians( rot_angle ), glm::vec3( 0.0f, 1.0f, 0.0f ) );
		// around x axis
		_cube.u_model = glm::rotate( _cube.u_model, glm::radians( rot_angle ), glm::vec3( 1.0f, 0.0f, 0.0f ) );
		// Scaling
		_cube.u_model = glm::scale( _cube.u_model, glm::vec3( 1.0f, 1.0f, 1.0f ) );
		
	}

	void LabWork3::render()
	{
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glBindVertexArray( _cube.vao );
		glUseProgram( _program );
		_mvp = _viewProj * _cube.u_model;
		glProgramUniformMatrix4fv( _program, u_mvp_location, 1, GL_FALSE, glm::value_ptr( _mvp ) );
		glDrawElements( GL_TRIANGLES, _cube.indices.size(), GL_UNSIGNED_INT, nullptr );
		glBindVertexArray( 0 );

	}

	void LabWork3::handleEvents( const SDL_Event & p_event )
	{
		if ( p_event.type == SDL_KEYDOWN )
		{
			switch ( p_event.key.keysym.scancode )
			{
			case SDL_SCANCODE_W: // Front
				_camera.moveFront( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_S: // Back
				_camera.moveFront( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_A: // Left
				_camera.moveRight( -_cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_D: // Right
				_camera.moveRight( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_R: // Up
				_camera.moveUp( _cameraSpeed );
				_updateViewMatrix();
				break;
			case SDL_SCANCODE_F: // Bottom
				_camera.moveUp( -_cameraSpeed );
				_updateViewMatrix();
				break;
			default: break;
			}
		}

		// Rotate when left click + motion (if not on Imgui widget).
		if ( p_event.type == SDL_MOUSEMOTION && p_event.motion.state & SDL_BUTTON_LMASK
			 && !ImGui::GetIO().WantCaptureMouse )
		{
			_camera.rotate( p_event.motion.xrel * _cameraSensitivity, p_event.motion.yrel * _cameraSensitivity );
			_updateViewMatrix();
		}

	}

	void LabWork3::displayUI()
	{
		ImGui::Begin( "Settings lab work 3" );

		if ( ImGui::CollapsingHeader( "General Settings" ) )
		{
			if ( ImGui::ColorEdit3( "Background", glm::value_ptr( _bgColor ) ) )
			{
				glClearColor( _bgColor.r, _bgColor.g, _bgColor.b, 1 );
			}
		}

		if ( ImGui::CollapsingHeader( "Camera Settings" ) )
		{
			ImGui::SliderFloat( "Camera Speed", &_cameraSpeed, 0, 10 );
			ImGui::SliderFloat( "Camera Sensitivity", &_cameraSensitivity, 0.1f, 5 );

			if ( ImGui::SliderFloat( "FOVY", &_fovy, 1, 120 ) )
			{
				_camera.setFovy( _fovy );
			}
		}

		if ( ImGui::CollapsingHeader( "Camera Info" ) )
		{
			ImGui::Text( "%s", _camera.getInfo().c_str() );
		}

		ImGui::End();
	}

	void LabWork3::_updateViewMatrix(void) { 
		_viewProj = _camera.getProjectionMatrix() * _camera.getViewMatrix();
		
	}

	

} // namespace M3D_ISICG
