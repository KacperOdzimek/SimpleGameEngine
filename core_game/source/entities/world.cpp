#include "world.h"

namespace entities
{
	void world::update()
	{
		dynamic_scene->update();
		for (auto& scene : scenes)
			scene->update();
	}

	void world::set_active_scene(std::unique_ptr<scene>* scene)
	{
		if (scene == nullptr)
			active_scene = &dynamic_scene;
		else
			active_scene = scene;
	}

	std::unique_ptr<scene>* world::get_active_scene()
	{
		return active_scene;
	}
}