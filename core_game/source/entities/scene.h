#pragma once
#include "entity.h"

#include <vector>

namespace entities
{
	class scene
	{
	protected:
		std::vector<entity*> entities;
	public:
		void add_entity(entity* entity);
		~scene();
	};
}