#include "sprite.h"

#include "source/entities/entity.h"

#include "source/common/common.h"
#include "source/assets/assets_manager.h"
#include "source/physics/collision_solver.h"
#include "source/rendering/renderer.h"

#include "source/utilities/hash_string.h"
#include "source/assets/texture_asset.h"

using namespace entities;
using namespace components;

sprite::sprite(uint32_t _id, std::weak_ptr<assets::texture> _texture, physics::collision_preset preset)
	: component(_id),
	mesh(_id),
	collider(
		_id, preset, 
		{
			_texture.lock()->get_width() / common::pixels_per_world_unit / 2,
			_texture.lock()->get_height() / common::pixels_per_world_unit / 2
		}
	)
{
	rc.material = assets::cast_asset<assets::shader>(
		common::assets_manager->get_asset(utilities::hash_string("core/sprite_shader"))).lock();

	rc.mesh = assets::cast_asset<assets::mesh>(
		common::assets_manager->get_asset(utilities::hash_string("core/square_mesh"))).lock();

	rc.textures = { _texture.lock() };
}

void sprite::on_attach()
{
	common::renderer->register_mesh_component(this);
}

void sprite::pass_transformation(rendering::transformations_buffer_iterator& tbi)
{
	if (visible)
	{
		glm::vec2 location = owner->get_location();

		tbi.put(std::move(location.x));
		tbi.put(std::move(location.y));

		tbi.put(1.0f);
		tbi.put(1.0f);

		tbi.put(owner->layer);
	}
}

const rendering::render_config& sprite::get_render_config()
{
	return rc;
}

sprite::~sprite()
{
	common::renderer->unregister_mesh_component(this);
}