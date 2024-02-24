#include "sprite_sheet.h"

namespace assets
{
	sprite_sheet::sprite_sheet(filesystem::image_file* data, unsigned int _sprite_width, unsigned int _sprite_height)
		: texture(data), sprite_width(_sprite_width), sprite_height(_sprite_height)
	{
	}

	sprite_sheet::~sprite_sheet()
	{
	}
}