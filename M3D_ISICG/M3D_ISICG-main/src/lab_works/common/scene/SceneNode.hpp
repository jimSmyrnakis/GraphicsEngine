#pragma once
#include "GL/gl3w.h"
#include "define.hpp"
#include <vector>
#include "../models/triangle_mesh.hpp"
namespace M3D_ISICG
{
	class SceneNode
	{

		protected:
			glm::mat4 m_localTransform;
			glm::mat4 m_worldTransform;
			SceneNode * m_parent;
			TriangleMesh * m_mesh;
			
	
	};
}
