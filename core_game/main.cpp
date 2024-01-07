#include "source/common/common.h"
#include "source/common/abort.h"

#include "source/rendering/renderer.h"
#include "source/assets/assets_manager.h"
#include "source/entities/world.h"

#include "source/entities/geometry_component.h"
#include "source/assets/texture_asset.h"
#include "source/utilities/hash_string.h"

int main()
{
	common::renderer->create_window();
	common::renderer->create_api_instance();

	common::assets_manager->set_assets_path("C:/Projekty/TopDownGame/mods/example_mod/assets");
	common::assets_manager->load_asset_from_json("/cat_shader.json");
	common::assets_manager->load_asset_from_json("/cat_texture.json");

	auto* scene = common::world->create_active_scene();
	auto entity = new entities::entity;

	std::weak_ptr<assets::shader> shader = assets::cast_asset<assets::shader>(
		common::assets_manager->get_asset(utilities::hash_string("/cat_shader.json")));

	std::weak_ptr<assets::texture> texture = assets::cast_asset<assets::texture>(
		common::assets_manager->get_asset(utilities::hash_string("/cat_texture.json")));

	auto geo_comp =
	new entities::test_geometry_component
	(
		entities::geometry_draw_settings{ shader, { texture } }
	);
	entity->attach_component(
		geo_comp
	);

	entity->position = { -0.75f, 0.75f };

	scene->add_entity(entity);
	while (!common::renderer->should_window_close())
	{
		common::renderer->collect_geometry_data();
		common::renderer->render();
		common::renderer->update_window();
		entity->position.y -= 0.001;
		entity->position.x += 0.001;
	}
	common::world.reset();
	common::assets_manager.reset();
	common::renderer.reset();
}