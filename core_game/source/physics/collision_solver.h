#pragma once
#include "source/components/collider.h"
#include "collision_present.h"
#include "include/glm/glm.hpp"

namespace physics
{
	class collision_solver
	{
	public:
		collision_event check_if_in_ray_dir(
			collision_present trace_present, glm::vec2 trace_begin, glm::vec2 trace_dir, entities::components::collider* collider);

		collision_present gen_flag(uint8_t body_type, std::array<collision_response, 14> responses);
		collision_response determine_response_type(const collision_present& f1, const collision_present& f2);
	};
}