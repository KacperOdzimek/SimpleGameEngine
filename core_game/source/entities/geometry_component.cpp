#include "geometry_component.h"

void entities::test_geometry_component::push_geometry(rendering::vertices_buffer_iterator& vertices_buffer, rendering::indicies_buffer_iterator& indicies_buffer)
{
	if (visible)
	{
		vertices_buffer.put(-0.5f + owner->position.x);
		vertices_buffer.put(-0.5f + owner->position.y);

		vertices_buffer.put(0.0f);
		vertices_buffer.put(0.0f);

		vertices_buffer.put(-0.5f + owner->position.x);
		vertices_buffer.put(0.5f + owner->position.y);

		vertices_buffer.put(0.0f);
		vertices_buffer.put(1.0f);

		vertices_buffer.put(0.5f + owner->position.x);
		vertices_buffer.put(-0.5f + owner->position.y);

		vertices_buffer.put(1.0f);
		vertices_buffer.put(0.0f);

		vertices_buffer.put(0.5f + owner->position.x);
		vertices_buffer.put(0.5f + owner->position.y);

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