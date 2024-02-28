#pragma once
#include "source/entities/scene.h"
#include <list>

namespace entities
{
	class entity;

	class world
	{
	friend entity;
	protected:
		std::unique_ptr<scene> dynamic_scene;
		std::list<std::unique_ptr<scene>> scenes;
	public:
		world() 
		{ 
			dynamic_scene = std::make_unique<scene>(); 
		}
		void destroy();
		void update();
		void create_scene(std::weak_ptr<assets::scene> scene, glm::vec2 position);
		scene* get_dynamic_scene();
	};
}