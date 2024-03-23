#pragma once
#include "texture_asset.h"
#include <vector>

namespace assets
{
	struct tilemap : public asset
	{
	public:
		const unsigned int width;
		const unsigned int height;

		using row = std::vector<int>;
		using layer = std::vector<row>;

		const std::vector<layer> layers;

		tilemap(unsigned int _width, unsigned int _height, std::vector<layer>& _tiles);
		~tilemap();
	};
}