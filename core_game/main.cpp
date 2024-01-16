#include "source/common/common.h"
#include "source/common/abort.h"

#include "source/rendering/renderer.h"
#include "source/assets/assets_manager.h"
#include "source/entities/world.h"
#include "source/behaviors/behaviors_manager.h"

#include <time.h>

//Can be removed in final version
#include "source/entities/geometry_component.h"
#include "source/components/camera.h"
#include "source/components/behavior.h"
#include "source/components/sprite.h"

#include "source/assets/texture_asset.h"
#include "source/assets/behavior_asset.h"
#include "source/assets/shader_asset.h"

#include "source/utilities/hash_string.h"
//

int main()
{
	common::renderer->create_window();
	common::renderer->create_api_instance();

	common::assets_manager->set_assets_path("C:/Projekty/TopDownGame/mods/example_mod/assets");

	common::assets_manager->load_asset_from_json("/shaders/cat_shader.json");
	common::assets_manager->load_asset_from_json("/textures/cat_texture.json");
	common::assets_manager->load_asset_from_json("/behaviors/move_right.json");
	common::assets_manager->load_asset_from_json("/behaviors/move_left.json");

	auto* scene = common::world->create_active_scene();

	/*
		The Cat
	*/
	auto cat_entity = new entities::entity;
	cat_entity->attach_component(
		new entities::components::sprite
		(
			utilities::hash_string("geo"),
			entities::geometry_draw_settings
			{ 
				assets::cast_asset<assets::shader>(common::assets_manager->get_asset(utilities::hash_string("/shaders/cat_shader.json"))), 
				{ 
					assets::cast_asset<assets::texture>(common::assets_manager->get_asset(utilities::hash_string("/textures/cat_texture.json"))) 
				} 
			}
		)
	);
	auto bhv = new entities::components::behavior{
			utilities::hash_string("behavior"),
			assets::cast_asset<assets::behavior>(common::assets_manager->get_asset(utilities::hash_string("/behaviors/move_right.json"))) };
	cat_entity->attach_component(
			bhv
	);
	/*auto bhv2 = new entities::components::behavior{
		utilities::hash_string("behavior"),
		assets::cast_asset<assets::behavior>(common::assets_manager->get_asset(utilities::hash_string("/behaviors/move_right.json"))) };
	cat_entity->attach_component(
		bhv2
	);*/

	/*
		Camera Actor
	*/
	auto camera_entity = new entities::entity;
	auto camera_comp = new entities::components::camera{ utilities::hash_string("cam"), 16 };
	camera_entity->attach_component(camera_comp);
	common::renderer->set_active_camera(camera_comp);
	camera_entity->position = { 0, 0 };

	while (!common::renderer->should_window_close())
	{
		double frame_start = ((double)clock()) / (double)CLOCKS_PER_SEC;

		common::behaviors_manager->call_update_functions();

		common::renderer->collect_geometry_data();
		common::renderer->render();
		common::renderer->update_window();

		double frame_end = ((double)clock()) / (double)CLOCKS_PER_SEC;
		common::delta_time = frame_end - frame_start;
	}
	common::world.reset();
	common::assets_manager.reset();
	common::renderer.reset();
}