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
		~scene();
		void add_entity(entity* entity);
		bool remove_entity(entity* entity);
	};
}