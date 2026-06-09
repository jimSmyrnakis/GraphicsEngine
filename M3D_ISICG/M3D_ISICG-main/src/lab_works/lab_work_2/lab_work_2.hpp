#ifndef __LAB_WORK_2_HPP__
#define __LAB_WORK_2_HPP__

#include "GL/gl3w.h"
#include "common/base_lab_work.hpp"
#include "define.hpp"
#include <vector>

namespace M3D_ISICG
{
	class LabWork2 : public BaseLabWork
	{
	  public:
		LabWork2() : BaseLabWork() {}
		~LabWork2();

		bool init() override;
		void animate( const float p_deltaTime ) override;
		void render() override;

		void handleEvents( const SDL_Event & p_event ) override;
		void displayUI() override;

	  private:
		// ================ Scene data.
		// ================

		// ================ GL data.
		std::vector<Vec2f> mesh;
		GLuint			   vbo_positions;
		GLuint			   ebo;	
		GLuint			   vbo_colors;
		GLuint			   vao;
		GLuint _program;
		std::vector<int>   indexBuffer; 
		std::vector<Vec4f> colorBuffer;
		GLuint			   uTranslationX_location;
		GLuint			   u_brightness_location;
		float			   uTranslationX_variable;
		float			   _time=0;
		// ================

		// ================ Settings.
		Vec4f _bgColor = Vec4f( 0.8f, 0.8f, 0.8f, 1.f ); // Background color
		float _Brightness = 1;
		// ================

		static const std::string _shaderFolder;
	};
} // namespace M3D_ISICG

#endif // __LAB_WORK_1_HPP__
