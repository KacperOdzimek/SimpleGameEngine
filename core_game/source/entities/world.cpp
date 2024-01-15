#include "world.h"

namespace entities
{
	scene* world::create_active_scene()
	{
		active_scene = std::make_unique<scene>();
		return active_scene.get();
	}
}