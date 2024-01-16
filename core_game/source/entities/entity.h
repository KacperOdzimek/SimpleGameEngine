#pragma once
#include "component.h"

#include "include/glm/glm.hpp"

#include <memory>
#include <vector>

namespace entities
{
	class component;

	class entity
	{
	friend class scene;
	friend component;
	protected:
		std::vector<component*> components;
		std::shared_ptr<entity> self;
	public:
		entity();
		void kill();
		glm::vec2 position{ 0.0f, 0.0f };;
		void attach_component(component* comp);
		component* get_component(uint32_t id);
		void kill_component(uint32_t id);
		//Make protected and shared a friend
		~entity();
	};
}