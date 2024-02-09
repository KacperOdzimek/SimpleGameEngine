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

namespace assets
{
	struct texture : public asset
	{
		friend rendering::renderer;
	private:
		graphics_abstraction::texture* _texture;
	public:
		//data is required to be filesystem::image_file*
		texture(void* data);
		~texture();
		unsigned int get_width();
		unsigned int get_height();
	};
}