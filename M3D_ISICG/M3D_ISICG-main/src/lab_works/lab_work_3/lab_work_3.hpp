#ifndef __LAB_WORK_3_HPP__
#define __LAB_WORK_3_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	struct Mesh
	{
		std::vector<Vec3f>		  positions;
		std::vector<Vec3f>		  colors;
		std::vector<unsigned int> indices;
		glm::mat4				  u_mvp;
		GLuint					  u_mvp_location;
		GLuint					  vbo_positions;
		GLuint					  vbo_colors;
		GLuint					  vao;
		GLuint					  ebo;

		void _createCube( void );
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
	  
	  private:
		
		// ================ Scene data.
		// ================

		// ================ GL data.
		Mesh			   _cube;
		GLuint			   _program;
		std::vector<int>   indexBuffer;
		
		GLuint			   u_brightness_location;
		float			   _time = 0;
		// ================

		// ================ Settings.
		Vec4f _bgColor	  = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		float _Brightness = 1;
		// ================

		static const std::string _shaderFolder;
	};
}; 

#endif 
