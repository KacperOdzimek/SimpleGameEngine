#include "world.h"

namespace entities
{
	scene* world::create_active_scene()
	{
		active_scene = std::make_unique<scene>();
		return active_scene.get();
	}

	void world::remove_entity_pointer(entity* e)
	{
		if (!active_scene->remove_entity(e))
			persistent_scene->remove_entity(e);
	}
}