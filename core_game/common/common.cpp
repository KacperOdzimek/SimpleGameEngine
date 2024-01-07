#include "common.h"
#include "source/engine/renderer.h"
#include "source/engine/assets_manager.h"
#include "source/engine/world.h"

namespace common
{
	std::string current_mod_path = "";
	std::unique_ptr<rendering::renderer> renderer = std::make_unique<rendering::renderer>();
	std::unique_ptr<assets::assets_manager> assets_manager = std::make_unique<assets::assets_manager>();
	std::unique_ptr<entities::world> world = std::make_unique<entities::world>();
}