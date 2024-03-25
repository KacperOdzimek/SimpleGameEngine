#include "common.h"

#include "source/rendering/renderer.h"
#include "source/rendering/flipbooks_manager.h"
#include "source/audio/audio_manager.h"
#include "source/assets/assets_manager.h"
#include "source/entities/world.h"
#include "source/behaviors/behaviors_manager.h"
#include "source/input/input_manager.h"
#include "source/window/window_manager.h"
#include "source/physics/collision_solver.h"
#include "source/physics/dynamics_manager.h"
#include "source/mods/mods_manager.h"

namespace common
{
	double delta_time = 0.0;
	double pixels_per_world_unit = 32;
	float physics_time_dilation_mod = 1.0f;
	std::unique_ptr<rendering::renderer> renderer = std::make_unique<rendering::renderer>();
	std::unique_ptr<rendering::flipbooks_manager> flipbooks_manager = std::make_unique<rendering::flipbooks_manager>();
	std::unique_ptr<audio::audio_manager> audio_manager = std::make_unique<audio::audio_manager>();;
	std::unique_ptr<assets::assets_manager> assets_manager = std::make_unique<assets::assets_manager>();
	std::unique_ptr<entities::world> world = std::make_unique<entities::world>();
	std::unique_ptr<behaviors::behaviors_manager> behaviors_manager = std::make_unique<behaviors::behaviors_manager>();
	std::unique_ptr<input::input_manager> input_mananger = std::make_unique<input::input_manager>();
	std::unique_ptr<window::window_manager> window_manager = std::make_unique<window::window_manager>();
	std::unique_ptr<physics::collision_solver> collision_solver = std::make_unique<physics::collision_solver>();
	std::unique_ptr<physics::dynamics_manager> dynamics_manager = std::make_unique<physics::dynamics_manager>();
	std::unique_ptr<mods::mods_manager> mods_manager = std::make_unique<mods::mods_manager>();
}