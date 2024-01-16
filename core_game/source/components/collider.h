#pragma once
#include "source/entities/component.h"
#include "include/glm/glm.hpp"
#include "source/physics/collision_present.h"

namespace entities
{
	namespace components
	{
		class collider : public component
		{
		public:
			physics::collision_present present = 0;
			collider(uint32_t _id, physics::collision_present _present, glm::vec2 _extend) 
				: component(_id), extend(_extend), present(_present) {};
			virtual void on_attach() {};
			glm::vec2 entity_offset = { 0,0 };
			glm::vec2 get_world_pos();
			glm::vec2 extend;
		private:
		};
	}
}