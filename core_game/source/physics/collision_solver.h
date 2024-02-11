#pragma once
#include "source/components/collider.h"
#include "collision.h"
#include "include/glm/vec2.hpp"

namespace physics
{
	class collision_solver
	{
		struct implementation;
		implementation* impl;
	public:
		collision_solver();
		~collision_solver();

		/*
			register_collider
			makes collider visible to collision detection system
		*/
		void register_collider(entities::components::collider* c);
		/*
			unregister_collider
			makes collider invisible to collision detection system
		*/
		void unregister_collider(entities::components::collider* c);
		/*
			check_if_ray_collide
			checks if ray of infinite length would hit the collider
		*/
		collision_event check_if_ray_collide(
			collision_preset trace_preset, glm::vec2 trace_begin, glm::vec2 trace_dir, entities::components::collider* collider);
		/*
			check_if_collider_collide_on_move
			checks wheter collider would hit another collider when moved to some location
		*/
		collision_event check_if_collider_collide_on_move(
			entities::components::collider* moved_collider, glm::vec2 target_location, entities::components::collider* other);
		/*
			sweep_move
			simulate collider move to some point
			returns structure containing all events that will occur
		*/
		sweep_move_event sweep_move(
			entities::components::collider* collider, glm::vec2& end_point);
	};
}