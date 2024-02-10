#pragma once
#include "source/entities/component.h"
#include "include/glm/glm.hpp"
#include "source/physics/collision.h"

namespace entities
{
	namespace components
	{
		class collider : public component
		{
		public:
			physics::collision_preset preset = 0;
			int layer_offset = 0;

			collider(uint32_t _id, physics::collision_preset _preset, glm::vec2 _extend) 
				: component(_id), extend(_extend), preset(_preset) {};
			~collider();

			virtual void on_attach();
			glm::vec2 entity_offset = { 0,0 };
			glm::vec2 get_world_pos();
			int get_layer();
			glm::vec2 extend;
		private:
		};
	}
}