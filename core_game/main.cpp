#include "source/common/common.h"

#include "source/assets/assets_manager.h"
#include "source/filesystem/filesystem.h"
#include "source/entities/world.h"
#include "source/rendering/renderer.h"
#include "source/behaviors/behaviors_manager.h"
#include "source/physics/collision_solver.h"
#include "source/input/input_manager.h"
#include "source/window/window_manager.h"

#include <time.h>

//Can be removed in final version
#include "source/components/mesh.h"
#include "source/components/camera.h"
#include "source/components/behavior.h"
#include "source/components/collider.h"

#include "source/assets/texture_asset.h"
#include "source/assets/behavior_asset.h"
#include "source/assets/shader_asset.h"

#include "source/utilities/hash_string.h"

#include "source/physics/collision.h"
#include <iostream>
//

int main()
{
	common::window_manager->create_window("Top Down Game", 16 * 80, 9 * 80, false);
	common::renderer->initialize();
	common::window_manager->set_resize_callback(common::renderer->get_resize_function());

	filesystem::set_mod_asset_path("C:/Projekty/TopDownGame/mods/example_mod/");
	filesystem::set_core_asset_path("C:/Projekty/TopDownGame/core_game/assets");

	/*
		Core Assets
	*/
	common::assets_manager->load_asset("core/square_mesh");
	common::assets_manager->lock_asset(utilities::hash_string("core/square_mesh"));

	common::assets_manager->load_asset("core/sprite_shader");
	common::assets_manager->lock_asset(utilities::hash_string("core/sprite_shader"));

	/*
		Mod Configuration
	*/
	common::assets_manager->load_asset("mod/collision_config");
	common::assets_manager->lock_asset(utilities::hash_string("mod/collision_config"));

	common::assets_manager->load_asset("mod/input_config");
	common::assets_manager->lock_asset(utilities::hash_string("mod/input_config"));

	common::world->create_active_scene();

	/*
		TEMP
	*/

	common::assets_manager->load_asset("mod/textures/sprite_sheet");
	common::assets_manager->lock_asset(utilities::hash_string("mod/textures/sprite_sheet"));

	auto create_entity_with_bhv = [&](glm::vec2 pos, std::string beh_path)
	{
		auto e = new entities::entity;
		e->teleport(pos);

		e->attach_component(
			new entities::components::behavior
			{
				utilities::hash_string("bhv"),
				assets::cast_asset<assets::behavior>(common::assets_manager->safe_get_asset(beh_path)).lock()
			}
		);

		return e;
	};

	auto b1 = create_entity_with_bhv({0.0f, 0.0f}, "mod/behaviors/move_left");
	auto b2 = create_entity_with_bhv({1.0f, 0.0f}, "mod/behaviors/move_right");

	/*
		Camera Actor
	*/
	auto camera_entity = new entities::entity;
	auto camera_comp = new entities::components::camera{ utilities::hash_string("cam"), 32 };
	camera_entity->attach_component(camera_comp);
	common::renderer->set_active_camera(camera_comp);
	camera_comp->rendered_layers = 2;
	camera_entity->layer = 0;
	camera_entity->teleport({ 0.0f, 0.0f });

	auto input_config = assets::cast_asset<assets::input_config>
		(common::assets_manager->get_asset(utilities::hash_string("mod/input_config")));
	common::input_mananger->load_config(input_config.lock());

	while (!common::window_manager->should_close())
	{
		double frame_start = ((double)clock()) / (double)CLOCKS_PER_SEC;

		//Remove expired entities Pointers
		common::world->update();

		//Game Logic
		common::input_mananger->update_mappings_states();
		common::behaviors_manager->call_update_functions();

		//Rendering
		common::renderer->update_transformations();
		common::renderer->render();
		common::window_manager->change_frame();

		//Remove dont used assets
		common::assets_manager->unload_unreferenced_assets();

		double frame_end = ((double)clock()) / (double)CLOCKS_PER_SEC;
		common::delta_time = frame_end - frame_start;
	}
	common::world.reset();
	common::assets_manager.reset();
	common::renderer.reset();
}