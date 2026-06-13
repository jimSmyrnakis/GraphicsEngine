#ifndef __LAB_WORK_5_HPP__
#define __LAB_WORK_5_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include "common/models/triangle_mesh.hpp"
#include "common/models/triangle_mesh_model.hpp"
#include <vector>
#include "../common/camera.hpp"

namespace M3D_ISICG
{
	
	class LabWork5 : public BaseLabWork
	{
	  public:
		LabWork5() : BaseLabWork() {}
		~LabWork5();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;
		void _updateViewMatrix(void);
	  private:
		
		// ================ Scene data.
		glm::vec3 _lightPosition = glm::vec3( 513, 566, -124 );
		// ================

		// ================ GL data.
		Camera	  _camera;
		float	  _cameraSensitivity = 1;
		float	  _cameraSpeed		 = 5;
		float	  _fovy				 = 65;
		glm::mat4 _mvp;
		glm::mat4 _viewProj;
		glm::mat4		  _tivMat;
		GLuint	  u_mvp_location;
		GLuint	  _program;
		GLuint			  u_mv_location;
		GLuint			  u_lightPosition_location;
		TriangleMeshModel _model;
		GLuint			  u_vMat_location;
		GLuint			  u_tivMat_location;
		// ================

		// ================ Settings.
		Vec4f _bgColor	  = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		// ================

		static const std::string _shaderFolder;
		static const std::string _modelFolder;
	};
}; 

#endif 
