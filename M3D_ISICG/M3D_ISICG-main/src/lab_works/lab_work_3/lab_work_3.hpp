#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>
#include "../common/camera.hpp"

namespace M3D_ISICG
{
	struct Mesh
	{
		std::vector<Vec3f>		  positions;
		std::vector<Vec3f>		  colors;
		std::vector<unsigned int> indices;
		glm::mat4				  u_model;
		GLuint					  vbo_positions;
		GLuint					  vbo_colors;
		GLuint					  vao;
		GLuint					  ebo;

		void _createCube( GLuint program );
	};
	class LabWork3 : public BaseLabWork
	{
	  public:
		LabWork3() : BaseLabWork() {}
		~LabWork3();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;
		void _updateViewMatrix(void);
	  private:
		
		// ================ Scene data.
		// ================

		// ================ GL data.
		Mesh			   _cube;
		glm::mat4		   _viewProj;
		GLuint			   _program;
		Camera			   _camera;
		GLuint			   u_brightness_location;
		float			   _cameraSensitivity = 1;
		float			   _cameraSpeed		  = 5;
		glm::mat4		   _mvp;
		GLuint			   u_mvp_location;
		// ================

		// ================ Settings.
		Vec4f _bgColor	  = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		float _fovy = 65;
		// ================

		static const std::string _shaderFolder;
	};
}; 

#endif 
