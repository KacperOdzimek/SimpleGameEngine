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
		void update();
		void create_scene(uint32_t name, glm::vec2 world_offset, std::weak_ptr<assets::scene> scene);
		void remove_scene(uint32_t name);
		scene* get_scene(uint32_t name);
		scene* get_persistent_scene();
	};
}