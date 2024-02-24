#pragma once
#include "texture_asset.h"
#include "source/physics/collision.h"

namespace filesystem
{
	struct image_file;
}

namespace assets
{
	struct tileset : public texture
	{
	public:
		const unsigned int sprite_width;
		const unsigned int sprite_height;
		const std::vector<int> colliding_tiles;
		tileset(filesystem::image_file* data, 
			unsigned int _sprite_width, unsigned int _sprite_height, 
			std::vector<int> _colliding_tiles);
		~tileset();
	};
}