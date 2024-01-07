#include "texture_asset.h"
#include "source/common/common.h"
#include "source/rendering/renderer.h"
#include "graphics_abstraction/graphics_abstraction.h"

namespace assets
{
	texture::texture(load_data data)
	{
		auto api = common::renderer->get_api();
		auto tb = api->create_texture_builder();
		tb->texture_type = graphics_abstraction::texture_type::texture_2d;
		graphics_abstraction::texture_internal_format format = graphics_abstraction::texture_internal_format::rgb;
		switch (data.color_channels)
		{
		case 1: format = graphics_abstraction::texture_internal_format::r; break;
		case 2: format = graphics_abstraction::texture_internal_format::rg; break;
		case 3: format = graphics_abstraction::texture_internal_format::rgb; break;
		case 4: format = graphics_abstraction::texture_internal_format::rgba; break;
		}
		tb->source_format = format;
		tb->internal_format = format;
		tb->width = data.width;
		tb->height = data.height;
		tb->source_data_type = graphics_abstraction::input_data_type::unsigned_byte;
		tb->source_texture = data.image_source;
		_texture = reinterpret_cast<graphics_abstraction::texture*>(api->build(tb));
	}

	texture::~texture()
	{
		common::renderer->get_api()->free(_texture);
	}
}