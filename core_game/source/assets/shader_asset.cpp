#include "shader_asset.h"
#include "common/common.h"
#include "source/engine/renderer.h"
#include "graphics_abstraction/graphics_abstraction.h"
#include "source/utilities/hash_string.h"

namespace assets
{
	shader::shader(std::string& vertex_shader, std::string& pixel_shader,
		std::vector<uint32_t>& hashed_layout)
	{
		auto api = common::renderer->get_api();

		auto sb = api->create_shader_builder();
		sb->code = { vertex_shader, pixel_shader };
		_shader = reinterpret_cast<graphics_abstraction::shader*>(api->build(sb));

		auto vl = api->create_vertex_layout_builder();

		std::vector<graphics_abstraction::data_type> layout;
		for (int element : hashed_layout)
		{
			switch (element)
			{
			case utilities::hash_string("float"):
				layout.push_back(graphics_abstraction::data_type::Float);
				break;
			case utilities::hash_string("vec2"):
				layout.push_back(graphics_abstraction::data_type::vec2);
				break;
			case utilities::hash_string("vec3"):
				layout.push_back(graphics_abstraction::data_type::vec3);
				break;
			case utilities::hash_string("vec4"):
				layout.push_back(graphics_abstraction::data_type::vec4);
				break;
			}
		}
		vl->vertex_components = layout;
		vertex_layout = reinterpret_cast<graphics_abstraction::vertex_layout*>(api->build(vl));
	}

	shader::~shader()
	{
		common::renderer->get_api()->free(_shader);
		common::renderer->get_api()->free(vertex_layout);
	}
}