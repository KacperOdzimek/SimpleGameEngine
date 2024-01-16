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
		int frames_since_purged = 0;
		std::vector<std::weak_ptr<entity>> entities;
	public:
		void update();
		~scene(); //Make protected and expose to unique<scene>
	};
}