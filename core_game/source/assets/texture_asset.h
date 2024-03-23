#pragma once
#include "asset.h"

namespace rendering
{
	class renderer;
}

namespace graphics_abstraction
{
	struct texture;
}

namespace filesystem
{
	struct image_file;
}

namespace assets
{
	struct texture : public asset
	{
		friend rendering::renderer;
	protected:
		graphics_abstraction::texture* _texture;
	public:
		texture(filesystem::image_file* data);
		~texture();
		unsigned int get_width();
		unsigned int get_height();
	};
}