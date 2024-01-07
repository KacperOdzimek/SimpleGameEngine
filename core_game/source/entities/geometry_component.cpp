#include "geometry_component.h"

void entities::test_geometry_component::push_geometry(rendering::buffer_iterator& vertices_buffer, rendering::buffer_iterator& indicies_buffer)
{
	if (visible)
	{
		vertices_buffer.put(-0.5f + owner->position.x);
		vertices_buffer.put(-0.5f + owner->position.y);

		vertices_buffer.put(-0.5f + owner->position.x);
		vertices_buffer.put(0.5f + owner->position.y);

		vertices_buffer.put(0.5f + owner->position.x);
		vertices_buffer.put(-0.5f + owner->position.y);

		vertices_buffer.put(0.5f + owner->position.x);
		vertices_buffer.put(0.5f + owner->position.y);

		indicies_buffer.put(0);
		indicies_buffer.put(1);
		indicies_buffer.put(2);

		indicies_buffer.put(1);
		indicies_buffer.put(2);
		indicies_buffer.put(3);
	}
}