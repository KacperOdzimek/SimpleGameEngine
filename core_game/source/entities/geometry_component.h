#pragma once
#include "component.h"
#include "source/common/common.h"

#include "source/entities/entity.h"

#include "source/rendering/renderer.h"
#include "source/rendering/renderer_data_types.h"
#include "source/rendering/buffer_iterator.h"

#include "source/assets/shader_asset.h"

namespace entities
{
	struct geometry_draw_settings
	{
		std::shared_ptr<assets::shader> shader = nullptr;
		geometry_draw_settings(std::shared_ptr<assets::shader> _shader)
		{
			shader = _shader;
		}
	};

	class geometry_component : public component
	{
		friend rendering::renderer;
	public:
		bool visible = true;
		geometry_draw_settings draw_settings;

	protected:
		virtual void push_geometry(rendering::buffer_iterator& vertices_buffer, rendering::buffer_iterator& indicies_buffer) = 0;

		inline rendering::pipeline_config get_rendering_config()
		{
			rendering::pipeline_config x;
			x.shader = draw_settings.shader.get();
			return x;
		}

	public:
		void set_draw_settings(geometry_draw_settings new_settings)
		{
			common::renderer->unregister_geometry_component(this, get_rendering_config());
			draw_settings = new_settings;
			common::renderer->register_geometry_component(this, get_rendering_config());
		}
		geometry_component(geometry_draw_settings gds) :
			draw_settings(gds)
		{ 	
			draw_settings = gds;
			common::renderer->register_geometry_component(this, get_rendering_config());
		};
		virtual ~geometry_component()
		{
			common::renderer->unregister_geometry_component(this, get_rendering_config());
		}
	};

	class test_geometry_component : public geometry_component
	{
	protected:
	public:
		test_geometry_component(geometry_draw_settings gds) :geometry_component(gds) {};
		virtual void push_geometry(rendering::buffer_iterator& vertices_buffer, rendering::buffer_iterator& indicies_buffer);
		virtual ~test_geometry_component() {};
	};
}