#pragma once
#include <memory>
#include <string>

namespace rendering
{
	class renderer;
}

namespace assets
{
	class assets_manager;
}

namespace entities
{
	class world;
}

namespace common
{
	extern std::string current_mod_path;
	extern std::unique_ptr<rendering::renderer> renderer;
	extern std::unique_ptr<assets::assets_manager> assets_manager;
	extern std::unique_ptr<entities::world> world;
}