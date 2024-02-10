#include "camera.h"

#include "source/entities/entity.h"

#include <glm/gtc/matrix_transform.hpp>

namespace entities
{
	namespace components
	{
		void camera::on_attach()
		{

		}

		int camera::get_deepest_rendered_layer()
		{
			return owner->layer;
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