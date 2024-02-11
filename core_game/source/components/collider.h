#pragma once
#include "source/entities/component.h"
#include "include/glm/glm.hpp"
#include "source/physics/collision.h"

namespace entities
{
	namespace components
	{
		/*
			collider
			component that links the collision detection subsystem with game logic
			collider makes body collide / overlap with other bodies containing colliders
		*/
		class collider : public component
		{
		public:
			/*
				flag that determine how should collider interact with other colliders
			*/
			physics::collision_preset preset = 0;

			/*
				offsets collider from entity layer
			*/
			int layer_offset = 0;
			
			/*
				get_layer
				returns collider layer
			*/
			int get_layer();

			/*
				entity-relative location
			*/
			glm::vec2 entity_offset = { 0,0 };

			/*
				returns collider world space location
			*/
			glm::vec2 get_world_pos();

			/*
				collider box extend
			*/
			glm::vec2 extend;

			collider(uint32_t _id, physics::collision_preset _preset, glm::vec2 _extend) 
				: component(_id), extend(_extend), preset(_preset) {};
			~collider();

			virtual void on_attach();
		private:
		};
	}
}