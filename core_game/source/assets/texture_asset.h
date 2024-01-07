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
		struct load_data
		{
			void* image_source;
			int width, height;
			int color_channels;
		};

		texture(load_data data);
		~texture();
	};
}