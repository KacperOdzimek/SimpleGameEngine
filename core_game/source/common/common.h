#pragma once
#include <memory>
#include <string>

namespace rendering
{
	class renderer;
	class flipbooks_manager;
}

namespace audio
{
	class audio_manager;
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

namespace window
{
	class window_manager;
}

namespace input
{
	class input_manager;
}

namespace physics
{
	class dynamics_manager;
	class collision_solver;
}

namespace mods
{
	class mods_manager;
}

namespace common
{
	/*
		previous frame duration
	*/
	extern double delta_time;
	/*
		texture pixels per world unit
	*/
	extern double pixels_per_world_unit;
	extern std::unique_ptr<rendering::flipbooks_manager> flipbooks_manager;
	extern std::unique_ptr<rendering::renderer> renderer;
	extern std::unique_ptr<audio::audio_manager> audio_manager;
	extern std::unique_ptr<assets::assets_manager> assets_manager;
	extern std::unique_ptr<entities::world> world;
	extern std::unique_ptr<behaviors::behaviors_manager> behaviors_manager;
	extern std::unique_ptr<window::window_manager> window_manager;
	extern std::unique_ptr<input::input_manager> input_mananger;
	extern std::unique_ptr<physics::collision_solver> collision_solver;
	extern std::unique_ptr<physics::dynamics_manager> dynamics_manager;
	extern std::unique_ptr<mods::mods_manager> mods_manager;
}