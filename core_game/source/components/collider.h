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
			collider(uint32_t _id, physics::collision_preset _preset, glm::vec2 _extend) 
				: component(_id), extend(_extend), preset(_preset) {};
			~collider();
			virtual void on_attach();
			void on_moved_to_other_chunk();
			glm::vec2 entity_offset = { 0,0 };
			glm::vec2 get_world_pos();
			glm::vec2 extend;
		private:
		};
	}
}