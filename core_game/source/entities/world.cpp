#include "world.h"
#include "source/common/crash.h"

namespace entities
{
	void world::destroy()
	{
		for (auto& scene : scenes)
			scene.reset();
		dynamic_scene.reset();
	}

	void world::update()
	{
		dynamic_scene->update();
		for (auto& scene : scenes)
			scene->update();
	}

	void world::create_scene(uint32_t name, std::weak_ptr<assets::scene> _scene)
	{
		for (auto& scene : scenes)
			if (scene->name == name)
			{
				error_handling::crash(error_handling::error_source::core, 
					"[world::create_scene]", "Scene named: " + std::to_string(name) + " already exists");
			}

		auto s = std::make_unique<scene>(name, _scene);
		scenes.push_back(std::move(s));
	}

	void world::remove_scene(uint32_t name)
	{
		for (auto& scene : scenes)
			if (scene->name == name)
			{
				scenes.remove(scene);
				return;
			}
	}

	scene* world::get_dynamic_scene()
	{
		return dynamic_scene.get();
	}
}