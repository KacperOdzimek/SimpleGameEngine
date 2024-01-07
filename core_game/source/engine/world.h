#pragma once
#include "source/entities/scene.h"

namespace entities
{
	class world
	{
		std::unique_ptr<scene> persistent_scene;
		std::unique_ptr<scene> active_scene;

	public:
		scene* create_active_scene()
		{
			active_scene = std::make_unique<scene>();
			return active_scene.get();
		}
	};
}