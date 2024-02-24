#include "tileset_asset.h"

namespace assets
{
	tileset::tileset(filesystem::image_file* data,
		unsigned int _tile_width, unsigned int _tile_height,
		std::vector<int> _colliding_tiles)
		: texture(data), tile_width(_tile_width), tile_height(_tile_height), colliding_tiles(_colliding_tiles)
	{
	}

	tileset::~tileset()
	{
	}
}