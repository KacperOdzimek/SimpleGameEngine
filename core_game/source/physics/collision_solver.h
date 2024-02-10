#pragma once
#include "source/components/collider.h"
#include "collision.h"
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
			collision_preset trace_preset, glm::vec2 trace_begin, glm::vec2 trace_dir, entities::components::collider* collider);
		collision_event check_if_collider_collide_on_move(
			entities::components::collider* moved_collider, glm::vec2 target_location, entities::components::collider* other);

		sweep_move_event sweep_move(
			entities::components::collider* collider, glm::vec2& end_point);
	};
}