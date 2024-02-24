#include "tilemap_asset.h"

namespace assets
{
	tilemap::tilemap(unsigned int _width, unsigned int _height, std::vector<layer>& _tiles)
		: width(_width), height(_height), layers(_tiles)
	{
	}

	tilemap::~tilemap() {};
}