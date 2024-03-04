#pragma once
#include "sprite_sheet.h"

namespace assets
{
	struct flipbook : public sprite_sheet
	{
	public: 
		struct animation
		{
			float frames_per_second;
			std::vector<int> frames;
		};

		const std::map<uint32_t, animation> animations;

		flipbook(filesystem::image_file* data, unsigned int _sprite_width, 
			unsigned int _sprite_height, std::map<uint32_t, animation> _animations);
		~flipbook();

		/*
			get_sprite_id_at_position
			returns current sprite in the given animation at given point of time
			playback_position < 0 or playback_position > animation length results in memory errors
		*/
		int get_sprite_id_at_position(uint32_t animation, float playback_position);
	};
}