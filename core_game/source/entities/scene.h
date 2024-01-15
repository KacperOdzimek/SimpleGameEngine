#pragma once
#include "entity.h"

#include <vector>
#include <memory>

namespace entities
{
	class entity;

	class scene
	{
		friend entity;
	protected:
		std::vector<std::shared_ptr<entity>> entities;
		std::weak_ptr<entity> add_entity(entity* e);
		bool remove_entity(std::shared_ptr<entity> e);
	public:
		~scene(); //Make protected and expose to unique<scene>
	};
}