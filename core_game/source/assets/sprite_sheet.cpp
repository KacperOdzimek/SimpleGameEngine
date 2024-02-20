#include "sprite_sheet.h"
#include "source/common/common.h"
#include "source/filesystem/filesystem.h"
#include "source/rendering/renderer.h"
#include "graphics_abstraction/graphics_abstraction.h"

namespace assets
{
	sprite_sheet::sprite_sheet(void* data, unsigned int _sprite_width, unsigned int _sprite_height)
		: texture(data), sprite_width(_sprite_width), sprite_height(_sprite_height)
	{
	}

	sprite_sheet::~sprite_sheet()
	{
	}
}