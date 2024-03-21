#pragma once 
#include "source/assets/sound_asset.h"

namespace audio
{
	class audio_manager
	{
	private:
		struct implementation;
		implementation* impl;
	public:
		audio_manager();
		~audio_manager();

		void play_sound(std::weak_ptr<assets::sound> sound);
	};
}