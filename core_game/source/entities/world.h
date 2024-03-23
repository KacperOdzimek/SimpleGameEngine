#pragma once
#include "source/entities/scene.h"

namespace entities
{
	class world
	{
	private:
		struct implementation;
		implementation* impl;
	public:
		world();
		~world();
		void destroy();
		void update();
		void create_scene(uint32_t name, std::weak_ptr<assets::scene> scene);
		void remove_scene(uint32_t name);
		scene* get_dynamic_scene();
	};
}