#pragma once
#include "asset.h"
#include <vector>

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
		//data is required to be filesystem::image_file*
		texture(filesystem::image_file* data);
		~texture();
		unsigned int get_width();
		unsigned int get_height();
	};
}