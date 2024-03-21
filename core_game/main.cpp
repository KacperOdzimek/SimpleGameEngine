#include "source/common/common.h"

#include "source/assets/assets_manager.h"
#include "source/entities/world.h"
#include "source/rendering/renderer.h"
#include "source/rendering/flipbooks_manager.h"
#include "source/behaviors/behaviors_manager.h"
#include "source/input/input_manager.h"
#include "source/window/window_manager.h"
#include "source/filesystem/filesystem.h"
#include "source/mods/mods_manager.h"
#include "source/physics/dynamics_manager.h"

#include "source/utilities/hash_string.h"

#include <time.h>

int main()
{
	common::window_manager->create_window("Top Down Game", 16 * 80, 9 * 80, false);
	common::renderer->initialize();
	common::window_manager->set_resize_callback(common::renderer->get_resize_function());

	filesystem::set_mods_directory("C:/Projekty/TopDownGame/mods/");
	filesystem::set_mod_assets_directory("C:/Projekty/TopDownGame/mods/example_mod/");
	filesystem::set_core_assets_directory("C:/Projekty/TopDownGame/core_game/assets");

	//Load required assets
	common::assets_manager->load_asset("core/square_mesh");
	common::assets_manager->lock_asset(utilities::hash_string("core/square_mesh"));
	common::assets_manager->load_asset("core/sprite_shader");
	common::assets_manager->lock_asset(utilities::hash_string("core/sprite_shader"));

	common::mods_manager->load_mod("C:/Projekty/TopDownGame/mods/game");

	while (!common::window_manager->should_close())
	{
		double frame_start = ((double)clock()) / (double)CLOCKS_PER_SEC;

		//Update flipbooks channels positions
		common::flipbooks_manager->update();

		//Game Logic
		common::world->update();
		common::input_mananger->update_mappings_states();
		common::behaviors_manager->call_update_functions();

		//Apply physics
		common::dynamics_manager->update();

		//Rendering
		common::renderer->update_transformations();
		common::renderer->render();
		common::window_manager->update();

		//Remove not used assets
		common::assets_manager->unload_unreferenced_assets();

		double frame_end = ((double)clock()) / (double)CLOCKS_PER_SEC;
		common::delta_time = frame_end - frame_start;
	}

	common::world->destroy();
	common::world.reset();
}