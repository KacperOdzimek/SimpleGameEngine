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
		std::unique_ptr<scene>* active_scene;
		std::unique_ptr<scene> dynamic_scene;
		std::list<std::unique_ptr<scene>> scenes;
	public:
		world() 
		{ 
			dynamic_scene = std::make_unique<scene>(); 
			active_scene = &dynamic_scene;
		}
		void update();
		/*
			set_active_scene
			sets active_scene so the scene can be accesed from behaviors
			pass nullptr to set it to dynamic_scene
		*/
		void set_active_scene(std::unique_ptr<scene>* scene);
		std::unique_ptr<scene>* get_active_scene();
	};
}