#include "mesh_asset.h"
#include "source/common/common.h"
#include "source/rendering/renderer.h"
#include "graphics_abstraction/graphics_abstraction.h"

assets::mesh::mesh(const std::vector<float>& _vertices, const std::vector<int>& _indicies)
	: draw_type(draw_type::indexed_triangles)
{
	auto api = common::renderer->get_api();

	auto bb = api->create_buffer_builder();

	bb->size = static_cast<uint32_t>(_vertices.size() * sizeof(float));
	bb->buffer_type = graphics_abstraction::buffer_type::vertex;
	vertices = reinterpret_cast<graphics_abstraction::buffer*>(api->build(bb, false));

	{
		auto* buffer_begin = vertices->open_data_stream();
		memcpy(buffer_begin, &_vertices.at(0), vertices->get_size());
		vertices->close_data_stream();
	}

	bb->size = static_cast<uint32_t>(_indicies.size() * sizeof(int));
	bb->buffer_type = graphics_abstraction::buffer_type::indicies;
	indicies = reinterpret_cast<graphics_abstraction::buffer*>(api->build(bb));

	{
		auto* buffer_begin = indicies->open_data_stream();
		memcpy(buffer_begin, &_indicies.at(0), indicies->get_size());
		indicies->close_data_stream();
	}	
}

assets::mesh::mesh(const std::vector<float>& _vertices)
	: draw_type(draw_type::triangle_strip), indicies(nullptr)
{
	auto api = common::renderer->get_api();
	auto bb = api->create_buffer_builder();
	bb->size = static_cast<uint32_t>(_vertices.size() * sizeof(float));
	bb->buffer_type = graphics_abstraction::buffer_type::vertex;
	vertices = reinterpret_cast<graphics_abstraction::buffer*>(api->build(bb));

	auto* buffer_begin = vertices->open_data_stream();
	memcpy(buffer_begin, &_vertices.at(0), vertices->get_size());
	vertices->close_data_stream();
}

assets::mesh::~mesh()
{
	auto api = common::renderer->get_api();
	api->free(vertices);
	if (indicies != nullptr)
		api->free(indicies);
}