#pragma once
#include "source/entities/scene.h"

namespace entities
{
	class entity;

	class world
	{
		friend entity;
		/*
			persistent_scene
			scene that dont get deleted
		*/
		std::unique_ptr<scene> persistent_scene;
		/*
			active_scene
			scene that can be deleted due to game logic
		*/
		std::unique_ptr<scene> active_scene;
	public:
		scene* create_active_scene();
		void update();
	};
}