#pragma once
#include "component.h"

#include "include/glm/glm.hpp"

#include <memory>
#include <vector>

namespace entities
{
	class entity
	{
	friend class scene;
	protected:
		std::vector<component*> components;
	public:
		glm::vec2 position{ 0.0f, 0.0f };;
		void attach_component(component* comp);
		component* get_component(uint32_t id);
		void kill_component(uint32_t id);
		void kill();
	protected:
		~entity();
	};
}