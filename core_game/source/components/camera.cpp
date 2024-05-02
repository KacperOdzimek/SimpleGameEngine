#include "camera.h"

#include "source/common/common.h"
#include "source/rendering/renderer.h"

#include "source/entities/entity.h"

#include <glm/gtc/matrix_transform.hpp>

namespace entities
{
	namespace components
	{
		void camera::on_attach()
		{
		}

		camera::~camera()
		{
			if (common::renderer->get_active_camera() == this)
				common::renderer->set_active_camera(nullptr);
		}

		glm::mat4 camera::get_projection()
		{
			constexpr float aspect = 9.0f / 16.0f;

			auto proj = glm::ortho(
				-ortho_width / 2, ortho_width / 2, 
				-ortho_width * aspect / 2, ortho_width * aspect / 2
			);
			
			return proj;
		}

		glm::vec2 camera::get_view_center_location()
		{
			return get_owner_location();
		}
	}
}