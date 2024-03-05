#include "flipbook_asset.h"

namespace assets
{
	flipbook::flipbook(filesystem::image_file* data, unsigned int _sprite_width, 
		unsigned int _sprite_height, std::map<uint32_t, animation> _animations)
		: sprite_sheet(data, _sprite_width, _sprite_height), animations(_animations)
	{
	}

	flipbook::~flipbook()
	{
	}

	int flipbook::get_sprite_id_at_position(uint32_t animation_id, float playback_position)
	{
		auto& animation = animations.at(animation_id);
		return animation.frames.at(floorf(playback_position / (1 / animation.frames_per_second)));
	}
}