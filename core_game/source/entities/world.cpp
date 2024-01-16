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
		active_scene->update();
		persistent_scene->update();
	}
}