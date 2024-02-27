#include "world.h"

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

	void world::set_active_scene(scene* scene)
	{
		if (scene == nullptr)
			active_scene = dynamic_scene.get();
		else
			active_scene = scene;
	}

	void world::create_scene(std::weak_ptr<assets::scene> _scene, glm::vec2 position)
	{
		auto s = std::make_unique<scene>(_scene);
		scenes.push_back(std::move(s));
	}

	scene* world::get_active_scene()
	{
		return active_scene;
	}
}