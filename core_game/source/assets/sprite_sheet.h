#pragma once
#include "texture_asset.h"

namespace assets
{
	struct sprite_sheet : public texture
	{
	public:
		const unsigned int sprite_width;
		const unsigned int sprite_height;
		sprite_sheet(filesystem::image_file* data, unsigned int _sprite_width, unsigned int _sprite_height);
		~sprite_sheet();
	};
}