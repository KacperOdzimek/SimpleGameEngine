#pragma once
#include "source/entities/scene.h"

namespace entities
{
	class entity;

	class world
	{
		friend entity;

		std::unique_ptr<scene> persistent_scene;
		std::unique_ptr<scene> active_scene;
	public:
		scene* create_active_scene();
	};
}