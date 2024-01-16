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

namespace behaviors
{
	class behaviors_manager;
}

namespace physics
{
	class collision_solver;
}

namespace common
{
	extern std::string current_mod_path;
	extern double delta_time;
	extern double pixels_per_world_unit;
	extern std::unique_ptr<rendering::renderer> renderer;
	extern std::unique_ptr<assets::assets_manager> assets_manager;
	extern std::unique_ptr<entities::world> world;
	extern std::unique_ptr<behaviors::behaviors_manager> behaviors_manager;
	extern std::unique_ptr<physics::collision_solver> collision_solver;
}