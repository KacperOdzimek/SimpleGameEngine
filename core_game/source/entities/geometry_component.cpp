#include "geometry_component.h"

namespace entities
{
	geometry_draw_settings::geometry_draw_settings(
		std::weak_ptr<assets::shader> _shader,
		std::vector<std::weak_ptr<assets::texture>> _textures)
	{
		shader = std::shared_ptr<assets::shader>(_shader);
		for (auto& text : _textures)
			textures.push_back(std::shared_ptr<assets::texture>(text));
	}

	geometry_component::geometry_component(uint32_t _id, geometry_draw_settings gds) :
		component(_id), draw_settings(gds)
	{
		draw_settings = gds;
	};

	geometry_component::~geometry_component()
	{
		common::renderer->unregister_geometry_component(this, get_rendering_config());
	}

	rendering::pipeline_config geometry_component::get_rendering_config()
	{
		rendering::pipeline_config x;
		x.shader = draw_settings.shader.get();
		for (auto& text : draw_settings.textures)
		{
			x.textures.push_back(text.get()->_texture);
		}
		return x;
	}

	void geometry_component::set_draw_settings(geometry_draw_settings new_settings)
	{
		common::renderer->unregister_geometry_component(this, get_rendering_config());
		draw_settings = new_settings;
		common::renderer->register_geometry_component(this, get_rendering_config());
	}

	void geometry_component::on_attach()
	{
		common::renderer->register_geometry_component(this, get_rendering_config());
	}
}