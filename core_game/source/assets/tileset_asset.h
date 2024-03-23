#pragma once
#include "texture_asset.h"
#include <vector>

namespace filesystem
{
	struct image_file;
}

namespace assets
{
	struct tileset : public texture
	{
	public:
		const unsigned int tile_width;
		const unsigned int tile_height;
		const std::vector<int> colliding_tiles;
		tileset(filesystem::image_file* data, 
			unsigned int _tile_width, unsigned int _tile_height,
			std::vector<int> _colliding_tiles);
		~tileset();
	};
}