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
#include "source/audio/audio_manager.h"

#include "source/common/crash.h"

#include "source/utilities/hash_string.h"

#include "source/physics/collision_solver.h"

constexpr double frame_time = (60.0f / 1000.0f);

#ifndef _DEBUG 
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#else
//Included in .gitignore, needs to be created for each clone
#include "debug_config.h"
#endif

int main()
{
	try
	{
		common::window_manager->create_window("Simple Game Engine", 16 * 80, 9 * 80, false);
		common::renderer->initialize();
		common::window_manager->set_resize_callback(common::renderer->get_resize_function());

#ifdef _DEBUG 
		{
			filesystem::set_mods_directory(debug_mods_directory);
			filesystem::set_core_assets_directory(debug_core_asssets_directory);
			filesystem::set_saved_directory(debug_saved_directory);
		}
#else
		{
			std::string path = filesystem::get_main_dir();
			filesystem::set_mods_directory(path + "mods/");
			filesystem::set_core_assets_directory(path + "assets/");
			filesystem::set_saved_directory(path + "saved/");
		}
#endif	

		//Load required assets
		common::assets_manager->load_required_core_assets();

#if _DEBUG
		common::mods_manager->load_mod(debug_loaded_mod);
#else
		common::mods_manager->load_mod_selection_mod();
#endif

		while (!common::window_manager->should_close())
		{
			double frame_start = ((double)clock()) / (double)CLOCKS_PER_SEC;

			bool should_quit_mod = 
				common::window_manager->get_key_state(input::get_key_from_key_name("Escape")).state == 1.0f &&
				common::mods_manager->get_current_mod_name() != "mod_selection";
			if (should_quit_mod)
				common::state = common::program_state::pending_for_mod_quit;

			//Exit or load another mod if requested
			switch (common::state)
			{
			case common::program_state::executing_logic: break;
			case common::program_state::pending_for_mod_load:
				common::mods_manager->unload_mod();
				common::mods_manager->load_mod(common::state_info);
				common::state = common::program_state::executing_logic;
				break;
			case common::program_state::pending_for_mod_quit:
				common::mods_manager->unload_mod();
				common::mods_manager->load_mod_selection_mod();
				common::state = common::program_state::executing_logic;
				break;
			};

			//Update flipbooks channels positions
			common::flipbooks_manager->update();

			//Audio
			common::audio_manager->update();

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

			//Adjust the frame rate
			double frame_end = ((double)clock()) / (double)CLOCKS_PER_SEC;
			common::window_manager->vsync(frame_end - frame_start);
			frame_end = ((double)clock()) / (double)CLOCKS_PER_SEC;
			common::delta_time = frame_end - frame_start;
			if (common::delta_time > frame_time) common::delta_time = frame_time;
		}
	}
	catch (const std::exception& exc)
	{
		error_handling::show_crash_info("[core][main]:\n Unhandled exception: " + std::string(exc.what()));
	}

	//ensure that entities are destroyed first, as their components 
	//holds shared pointers to almost every resource in the engine
	common::world.reset();

	//ensure that assets are destroyed second, as they owns resources, 
	//that can be only destroyed by other manager class
	common::assets_manager.reset();
}