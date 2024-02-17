#include "world.h"

namespace entities
{
	scene* world::create_active_scene()
	{
		active_scene = std::make_unique<scene>();
		return active_scene.get();
	}

	void world::update()
	{
		if (active_scene.get() != nullptr) active_scene->update();
		if (persistent_scene.get() != nullptr) persistent_scene->update();
	}
}