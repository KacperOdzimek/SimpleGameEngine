#pragma once
#include "source/entities/component.h"
#include "glm/glm.hpp"

namespace entities
{
	namespace components
	{
		class camera : public entities::component
		{
		public:
			camera(float _ortho_width) : ortho_width(_ortho_width) {}
			float ortho_width = 20;
			glm::mat4 get_projection();
			glm::vec2 get_view_center_location();
		private:
		};
	}
}