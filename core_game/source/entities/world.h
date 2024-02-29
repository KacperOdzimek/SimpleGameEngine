#pragma once
#include "source/entities/scene.h"
#include <list>

namespace entities
{
	class world
	{
	protected:
		std::unique_ptr<scene> dynamic_scene;
		std::list<std::unique_ptr<scene>> scenes;
	public:
		world() 
		{ 
			dynamic_scene = std::make_unique<scene>(0); 
		}
		void destroy();
		void update();
		void create_scene(uint32_t name, std::weak_ptr<assets::scene> scene);
		void remove_scene(uint32_t name);
		scene* get_dynamic_scene();
	};
}