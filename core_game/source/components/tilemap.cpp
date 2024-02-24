#include "tilemap.h"
#include "source/entities/entity.h"
#include "source/common/common.h"
#include "source/assets/assets_manager.h"
#include "source/rendering/renderer.h"
#include "source/utilities/hash_string.h"

using namespace entities;
using namespace components;

tilemap::tilemap(
	uint32_t _id,
	std::weak_ptr<assets::tilemap> _tilemap,
	std::weak_ptr<assets::tileset> _tileset
) : component(_id), mesh(_id),  tilemap_asset(_tilemap), tileset_asset(_tileset)
{
	_config.material = assets::cast_asset<assets::shader>(
		common::assets_manager->get_asset(utilities::hash_string("core/sprite_shader"))).lock();

	_config.mesh = assets::cast_asset<assets::mesh>(
		common::assets_manager->get_asset(utilities::hash_string("core/square_mesh"))).lock();

	_config.textures = { tileset_asset };
}

void tilemap::pass_transformation(rendering::transformations_buffer_iterator& tbi)
{
	int layer_counter = 0;

	float tile_x_size = (tileset_asset->tile_width / common::pixels_per_world_unit);
	float tile_y_size = (tileset_asset->tile_width / common::pixels_per_world_unit);

	int id_offset = 
		tileset_asset->get_width() / tileset_asset->tile_width * (tileset_asset->get_height() / tileset_asset->tile_width - 1) - 1;

	for (auto& layer : tilemap_asset->layers)
	{
		float y_mod = -1 * (float(tilemap_asset->height) / 2) * tile_y_size;
		auto row = layer.end() - 1;

		while (row != layer.begin())
		{
			float x_mod = -1 * (float(tilemap_asset->width) / 2) * tile_x_size;
			for (auto& tile : *row)
			{
				if (tile == 0)
				{
					x_mod += tile_x_size;
					continue;
				}

				tbi.put(owner->get_location().x + x_mod);
				tbi.put(owner->get_location().y + y_mod);

				tbi.put(tile_x_size);
				tbi.put(tile_y_size);

				tbi.put(owner->layer + layer_counter);
				tbi.put(tile + id_offset);

				x_mod += tile_x_size;
			}
			y_mod += tile_y_size;
			row--;
		}	
	}
}

const rendering::render_config& tilemap::get_render_config()
{
	return _config;
}

uint32_t tilemap::get_instances_amount()
{
	return tilemap_asset->width * tilemap_asset->height * tilemap_asset->layers.size();
}

tilemap::~tilemap()
{
	common::renderer->unregister_mesh_component(this);
}