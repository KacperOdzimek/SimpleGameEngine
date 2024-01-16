#include "sprite.h"
#include "source/common/common.h"
#include "source/assets/texture_asset.h"

void entities::components::sprite::push_geometry
	(rendering::vertices_buffer_iterator& vertices_buffer, rendering::indicies_buffer_iterator& indicies_buffer)
{
	if (draw_settings.textures.size() == 0)
		return;

	auto& txt = draw_settings.textures.at(0);
	auto extend_x = txt->get_width() / common::pixels_per_world_unit;
	auto extend_y = txt->get_height() / common::pixels_per_world_unit;

	if (visible)
	{
		vertices_buffer.put(-extend_x + owner->position.x + entity_offset.x);
		vertices_buffer.put(-extend_y + owner->position.y + entity_offset.y);

		vertices_buffer.put(0.0f);
		vertices_buffer.put(0.0f);

		vertices_buffer.put(-extend_x + owner->position.x + entity_offset.x);
		vertices_buffer.put(extend_y + owner->position.y + entity_offset.y);

		vertices_buffer.put(0.0f);
		vertices_buffer.put(1.0f);

		vertices_buffer.put(extend_x + owner->position.x + entity_offset.x);
		vertices_buffer.put(-extend_y + owner->position.y + entity_offset.y);

		vertices_buffer.put(1.0f);
		vertices_buffer.put(0.0f);

		vertices_buffer.put(extend_x + owner->position.x + entity_offset.x);
		vertices_buffer.put(extend_y + owner->position.y + entity_offset.y);

		vertices_buffer.put(1.0f);
		vertices_buffer.put(1.0f);

		indicies_buffer.put(0);
		indicies_buffer.put(1);
		indicies_buffer.put(2);

		indicies_buffer.put(1);
		indicies_buffer.put(2);
		indicies_buffer.put(3);

		indicies_buffer.offset(4);
	}
}