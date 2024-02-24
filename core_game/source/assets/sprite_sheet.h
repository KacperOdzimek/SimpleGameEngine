#pragma once
#include "texture_asset.h"

namespace assets
{
	struct sprite_sheet : public texture
	{
	public:
		const unsigned int sprite_width;
		const unsigned int sprite_height;
		//data is required to be filesystem::image_file*
		sprite_sheet(void* data, unsigned int _sprite_width, unsigned int _sprite_height);
		~sprite_sheet();
	};
}