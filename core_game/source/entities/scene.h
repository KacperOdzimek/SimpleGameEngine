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
		std::vector<std::weak_ptr<entity>> entities;
	public:
		~scene(); //Make protected and expose to unique<scene>
	};
}