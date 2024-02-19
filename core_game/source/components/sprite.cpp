#include "sprite.h"

#include "source/common/common.h"
#include "source/assets/assets_manager.h"
#include "source/physics/collision_solver.h"
#include "source/rendering/renderer.h"

#include "source/utilities/hash_string.h"
#include "source/assets/texture_asset.h"

using namespace entities;
using namespace components;

sprite::sprite(uint32_t _id, std::weak_ptr<assets::texture> _texture, physics::collision_preset preset)
	: texture(_texture.lock()),
	component(_id),
	mesh(
		_id,
		assets::cast_asset<assets::mesh>(common::assets_manager->get_asset(utilities::hash_string("core/square_mesh"))),
		assets::cast_asset<assets::shader>(common::assets_manager->get_asset(utilities::hash_string("core/sprite_shader"))),
		{
			_texture
		}
	),
	collider(
		_id, preset, 
		{
			_texture.lock()->get_width() / common::pixels_per_world_unit / 2,
			_texture.lock()->get_height() / common::pixels_per_world_unit / 2
		}
	)
{
	scale =
	{
		_texture.lock()->get_width() / common::pixels_per_world_unit,
		_texture.lock()->get_height() / common::pixels_per_world_unit
	};
}

void sprite::on_attach()
{
	common::collision_solver->register_collider(this);
	common::renderer->register_mesh_component(this);
}

sprite::~sprite()
{
}