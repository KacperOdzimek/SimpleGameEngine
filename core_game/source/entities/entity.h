#pragma once
#include "component.h"

#include "include/glm/glm.hpp"

#include <memory>
#include <vector>

namespace entities
{
	class entity
	{
	protected:
		std::vector<component*> components;
	public:
		glm::vec2 position;
		void attach_component(component* comp)
		{
			components.push_back(comp);
			comp->owner = this;
		}
		~entity()
		{
			for (auto& comp : components)
				delete comp;
		}
	};
}