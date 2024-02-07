#pragma once

#include "asset.h"
#include <vector>

namespace entities
{
	class geometry_component;
}

namespace graphics_abstraction
{
	struct texture;
}

namespace assets
{
	struct texture : public asset
	{
		friend entities::geometry_component;
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