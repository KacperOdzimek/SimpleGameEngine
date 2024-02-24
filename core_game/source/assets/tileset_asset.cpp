#include "tileset_asset.h"

namespace assets
{
	tileset::tileset(filesystem::image_file* data,
		unsigned int _sprite_width, unsigned int _sprite_height,
		std::vector<int> _colliding_tiles)
		: texture(data), sprite_width(_sprite_width), sprite_height(_sprite_height), colliding_tiles(_colliding_tiles)
	{
	}

	tileset::~tileset()
	{
	}
}