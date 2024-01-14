#pragma once
#include "component.h"
#include "source/common/common.h"

#include "source/entities/entity.h"

#include "source/rendering/renderer.h"
#include "source/rendering/renderer_data_types.h"
#include "source/rendering/buffer_iterator.h"

#include "source/assets/shader_asset.h"
#include "source/assets/texture_asset.h"

namespace rendering
{
	class renderer;
}

namespace entities
{
	struct geometry_draw_settings
	{
		std::shared_ptr<assets::shader> shader = nullptr;
		std::vector<std::shared_ptr<assets::texture>> textures;
		geometry_draw_settings(
			std::weak_ptr<assets::shader> _shader,
			std::vector<std::weak_ptr<assets::texture>> _textures);
	};

	class geometry_component : public component
	{
		friend rendering::renderer;
	public:
		bool visible = true;
		glm::vec2 entity_offset;
		geometry_draw_settings draw_settings;
	public:
		geometry_component(uint32_t _id, geometry_draw_settings gds);
		void set_draw_settings(geometry_draw_settings new_settings);		
	protected:
		virtual void push_geometry(rendering::vertices_buffer_iterator& vertices_buffer, rendering::indicies_buffer_iterator& indicies_buffer) = 0;
		rendering::pipeline_config get_rendering_config();
	public:
		virtual ~geometry_component();
		virtual void on_attach();
	};
}