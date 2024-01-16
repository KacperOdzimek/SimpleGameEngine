#pragma once
#include "source/entities/geometry_component.h"

namespace entities
{
	namespace components
	{
		class sprite : public geometry_component
		{
		protected:
		public:
			sprite(uint32_t _id, geometry_draw_settings gds) : geometry_component(_id, gds) {};
			virtual void push_geometry(rendering::vertices_buffer_iterator& vertices_buffer, rendering::indicies_buffer_iterator& indicies_buffer);
			virtual ~sprite() {};
		};
	}
}