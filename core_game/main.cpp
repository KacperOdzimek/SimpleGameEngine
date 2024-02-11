#include "source/common/common.h"
#include "source/common/abort.h"

#include "source/assets/assets_manager.h"
#include "source/filesystem/filesystem.h"
#include "source/entities/world.h"
#include "source/rendering/renderer.h"
#include "source/behaviors/behaviors_manager.h"
#include "source/physics/collision_solver.h"

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
#include <bitset>
//

int main()
{
	common::renderer->create_window();
	common::renderer->initialize();

	filesystem::set_mod_asset_path("C:/Projekty/TopDownGame/mods/example_mod/");
	filesystem::set_core_asset_path("C:/Projekty/TopDownGame/core_game/assets");

	common::assets_manager->load_asset("core/unit_square_mesh");
	common::assets_manager->lock_asset(utilities::hash_string("core/unit_square_mesh"));

	common::assets_manager->load_asset("mod/collision_config");
	common::assets_manager->lock_asset(utilities::hash_string("mod/collision_config"));

	common::assets_manager->load_asset("mod/input_config");
	common::assets_manager->lock_asset(utilities::hash_string("mod/input_config"));

	common::assets_manager->load_asset("mod/shaders/cat_shader");
	common::assets_manager->load_asset("mod/textures/cat_texture");
	common::assets_manager->load_asset("mod/behaviors/move_right");
	common::assets_manager->load_asset("mod/behaviors/move_left");

	common::world->create_active_scene();

	/*
		The Box
	*/
	auto create_spining_box = [&](glm::vec2 pos, std::string beh_path)
	{
		auto box = new entities::entity;
		box->teleport(pos);

		auto mesh = new entities::components::mesh{ 
			utilities::hash_string("mesh"), 
			assets::cast_asset<assets::mesh>(common::assets_manager->get_asset(utilities::hash_string("core/unit_square_mesh"))),
			assets::cast_asset<assets::shader>(common::assets_manager->get_asset(utilities::hash_string("mod/shaders/cat_shader"))),
			{
				assets::cast_asset<assets::texture>(common::assets_manager->get_asset(utilities::hash_string("mod/textures/cat_texture"))),
			}
		};

		box->attach_component(mesh);

		auto f = physics::gen_flag(0, { physics::collision_response::ignore });
		auto col = new entities::components::collider
		{
			utilities::hash_string("collider1"), f, {0.5f, 0.5f}
		};
		box->attach_component(
			col
		);

		box->attach_component(
			new entities::components::behavior
			{
				utilities::hash_string("bhv"),
				assets::cast_asset<assets::behavior>(common::assets_manager->get_asset(utilities::hash_string(beh_path))).lock()
			}
		);

		return std::pair<entities::entity*, entities::components::collider*>{ box, col };
	};

	auto b1 = create_spining_box({-1.0f, 0.0f}, "mod/behaviors/move_left");
	auto b2 = create_spining_box({1.0f, 0.0f}, "mod/behaviors/move_right");

	/*
		Camera Actor
	*/
	auto camera_entity = new entities::entity;
	auto camera_comp = new entities::components::camera{ utilities::hash_string("cam"), 16 };
	camera_entity->attach_component(camera_comp);
	common::renderer->set_active_camera(camera_comp);
	camera_comp->rendered_layers = 2;
	camera_entity->layer = 0;
	camera_entity->teleport({ 0.0f, 0.0f });

	while (!common::renderer->should_window_close())
	{
		double frame_start = ((double)clock()) / (double)CLOCKS_PER_SEC;

		common::behaviors_manager->call_update_functions();

		common::renderer->update_transformations();
		common::renderer->render();
		common::renderer->update_window();

		common::assets_manager->unload_unreferenced_assets();

		double frame_end = ((double)clock()) / (double)CLOCKS_PER_SEC;
		common::delta_time = frame_end - frame_start;
	}
	common::world.reset();
	common::assets_manager.reset();
	common::renderer.reset();
}