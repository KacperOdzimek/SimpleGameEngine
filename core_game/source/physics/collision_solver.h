#pragma once
#include "source/components/collider.h"
#include "collision_present.h"
#include "include/glm/glm.hpp"

namespace physics
{
	class collision_solver
	{
		struct implementation;
		implementation* impl;
	public:
		collision_solver();
		~collision_solver();

		void register_collider(entities::components::collider* c);
		void unregister_collider(entities::components::collider* c);

		collision_event check_if_ray_collide(
			collision_present trace_present, glm::vec2 trace_begin, glm::vec2 trace_dir, entities::components::collider* collider);
		collision_event check_if_colliders_collide(
			entities::components::collider* collider_1, entities::components::collider* collider_2);

		collision_event sweep_move(
			glm::vec2 starting_point, glm::vec2 end_point, entities::components::collider* collider);

		collision_present gen_flag(uint8_t body_type, std::array<collision_response, 14> responses);
		collision_response get_response_type(const collision_present& f1, const collision_present& f2);
	};
}