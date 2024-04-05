#include "sprite.h"

#include "source/entities/entity.h"

#include "source/common/common.h"
#include "source/assets/assets_manager.h"
#include "source/physics/collision_solver.h"
#include "source/rendering/renderer.h"

#include "source/assets/sprite_sheet.h"
#include "source/assets/rendering_config_asset.h"

#include "source/utilities/hash_string.h"

#include "source/assets/shader_asset.h"
#include "source/assets/mesh_asset.h"

using namespace entities;
using namespace components;

glm::vec2 get_extend_from_texture(const std::weak_ptr<assets::texture>& txt)
{
	auto ptr = txt.lock().get();
	auto casted = dynamic_cast<assets::sprite_sheet*>(ptr);
	if (casted != nullptr)
		return	{
			casted->sprite_width / common::pixels_per_world_unit,
			casted->sprite_height / common::pixels_per_world_unit
	};
	else
		return {
			ptr->get_width() / common::pixels_per_world_unit,
			ptr->get_height() / common::pixels_per_world_unit
		};
}

sprite::sprite(uint32_t _id, std::weak_ptr<assets::texture> _texture, physics::collision_preset preset)
	: component(_id),
	mesh(_id),
	collider(
		_id, preset, get_extend_from_texture(_texture) / 2.0f
	)
{
	auto rendering_config = assets::cast_asset<assets::rendering_config>(
		common::assets_manager->get_asset(utilities::hash_string("mod/rendering_config"))).lock();

	rc.material = assets::cast_asset<assets::shader>(
		common::assets_manager->safe_get_asset(rendering_config->default_sprite_shader)).lock();

	rc.mesh = assets::cast_asset<assets::mesh>(
		common::assets_manager->get_asset(utilities::hash_string("core/square_mesh"))).lock();

	rc.textures = { _texture.lock() };

	sprite_extend = get_extend_from_texture(_texture);
}

void sprite::on_attach()
{
	common::renderer->register_mesh_component(this);
	common::collision_solver->register_collider(this);
}

void sprite::set_sprite_id(int new_sprite_id)
{
	if (sprite_id != new_sprite_id)
	{
		sprite_id = new_sprite_id;
		common::renderer->mark_pipeline_dirty(this->get_render_config());
	}
}

void sprite::set_shader(std::weak_ptr<assets::shader> new_shader)
{
	common::renderer->unregister_mesh_component(this);
	rc.material = new_shader.lock();
	common::renderer->register_mesh_component(this);
}

void sprite::pass_transformation(rendering::transformations_buffer_iterator& tbi)
{
	if (visible)
	{
		glm::vec2 location = owner->get_location();

		tbi.put(std::move(location.x + offset.x));
		tbi.put(std::move(location.y + offset.y));

		tbi.put(sprite_extend.x * scale.x);
		tbi.put(sprite_extend.y * scale.y);

		tbi.put(owner->layer);
		tbi.put(sprite_id);
	}
}

const rendering::render_config& sprite::get_render_config()
{
	return rc;
}

uint32_t entities::components::sprite::get_instances_amount()
{
	return 1;
}

sprite::~sprite()
{
	common::renderer->unregister_mesh_component(this);
}