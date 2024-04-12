#include "world.h"
#include "source/common/crash.h"
#include <list>

namespace entities
{
	struct world::implementation
	{
		std::unique_ptr<scene> dynamic_scene;
		std::list<std::unique_ptr<scene>> scenes;
	};

	world::world()
	{
		impl = new implementation;
		impl->dynamic_scene = std::make_unique<scene>(0, glm::vec2(0, 0));
	}

	world::~world()
	{
		delete impl;
	}

	void world::update()
	{
		impl->dynamic_scene->update();
		for (auto& scene : impl->scenes)
			scene->update();
	}

	void world::create_scene(uint32_t name, glm::vec2 world_offset, std::weak_ptr<assets::scene> _scene)
	{
		for (auto& scene : impl->scenes)
			if (scene->name == name)
			{
				return;
			}

		auto s = std::make_unique<scene>(name, world_offset, _scene);
		impl->scenes.push_back(std::move(s));
	}

	void world::remove_scene(uint32_t name)
	{
		for (auto& scene : impl->scenes)
			if (scene->name == name)
			{
				impl->scenes.remove(scene);
				return;
			}
	}

	scene* world::get_scene(uint32_t name)
	{
		for (auto& scene : impl->scenes)
			if (scene->name == name)
				return scene.get();
		return nullptr;
	}

	scene* world::get_dynamic_scene()
	{
		return impl->dynamic_scene.get();
	}
}