#include "lab_work_5.hpp"
#include "imgui.h"
#include "utils/read_file.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>

namespace M3D_ISICG
{
	
		

	const std::string LabWork5::_shaderFolder = "src/lab_works/lab_work_5/assets/shaders/";
	const std::string LabWork5::_modelFolder  = "src/lab_works/lab_work_5/assets/models/";

	LabWork5::~LabWork5()
	{
		glDeleteProgram( _program );
		glDisable( GL_DEPTH_TEST );
	}

	bool LabWork5::init()
	{
		_camera.setFovy( 65 );
		_camera.setScreenSize( _windowWidth, _windowHeight );
		_camera.setPosition( glm::vec3( 0.f, 0.f, 0.f ) );
		_updateViewMatrix();

		std::cout << "Initializing lab work 4..." << std::endl;
		// Set the color used by glClear to clear the color buffer (in render()).
		glClearColor( _bgColor.x, _bgColor.y, _bgColor.z, _bgColor.w );
		
		// Vertex Shader
		std::string vertexShaderStr = readFile( _shaderFolder + "mesh.vert" );
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
		std::string fragmentShaderStr = readFile( _shaderFolder + "mesh.frag" );
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

		


		u_mvp_location = glGetUniformLocation( _program, "u_MVPMatrix" );
		u_mv_location  = glGetUniformLocation( _program, "u_MVMatrix" );
		u_lightPosition_location = glGetUniformLocation( _program, "u_lightPosition" );
		u_vMat_location			 = glGetUniformLocation( _program, "u_VMatrix" );
		u_tivMat_location		 = glGetUniformLocation( _program, "u_TIVMatrix" );

		glUseProgram( _program );
		glUniform3fv( u_lightPosition_location, 1, glm::value_ptr( _lightPosition ) );
		glUseProgram( 0 );

		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LESS );


		_model.load( "runtime_name", _modelFolder + "sponza.obj" );
		return true;
	}
	
	void LabWork5::animate( const float p_deltaTime )
	{ 
		_lightPosition = _camera.getPosition();
		glUseProgram( _program );
		glUniform3fv( u_lightPosition_location, 1, glm::value_ptr( _lightPosition ) );
		glUseProgram( 0 );
		
	}

	void LabWork5::render()
	{
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		glUseProgram( _program );
		_mvp = _viewProj * glm::mat4( 1.0 );
		glProgramUniformMatrix4fv( _program, u_mvp_location, 1, GL_FALSE, glm::value_ptr( _mvp ) );
		_tivMat = glm::transpose( glm::inverse( _camera.getViewMatrix() ) );
		glProgramUniformMatrix4fv( _program, u_tivMat_location, 1, GL_FALSE, glm::value_ptr( _tivMat ) );
		glProgramUniformMatrix4fv( _program, u_vMat_location, 1, GL_FALSE, glm::value_ptr( _camera.getViewMatrix() ) );
		glProgramUniformMatrix4fv( _program, u_mv_location, 1, GL_FALSE, glm::value_ptr( _camera.getViewMatrix() ) );
		_model.render( _program );
		glBindVertexArray( 0 );

	}

	void LabWork5::handleEvents( const SDL_Event & p_event )
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

	void LabWork5::displayUI()
	{
		ImGui::Begin( "Settings lab work 4" );

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
				_updateViewMatrix();
			}

			// znear and zfar settings
			if ( ImGui::SliderFloat( "Z Near", &_camera.getZnear(), 0.01f, 10.f ) )
			{
				_camera.SetZSpace( _camera.getZnear(), _camera.getZfar() );
				_updateViewMatrix();
			}

			if ( ImGui::SliderFloat( "Z Far", &_camera.getZfar(), 10.f, 10000.f ) )
			{
				_camera.SetZSpace( _camera.getZnear(), _camera.getZfar() );
				_updateViewMatrix();
			}
		}

		if ( ImGui::CollapsingHeader( "Camera Info" ) )
		{
			ImGui::Text( "%s", _camera.getInfo().c_str() );
		}

		ImGui::End();
	}

	void LabWork5::_updateViewMatrix()
	{
		glm::mat4 view = _camera.getViewMatrix();
		glm::mat4 proj = _camera.getProjectionMatrix();
		_viewProj	   = proj * view;
	}

	

	

} // namespace M3D_ISICG
