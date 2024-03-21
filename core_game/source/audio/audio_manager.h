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
		void set_volume(float volume_precent);

		void play_sound_at_channel(uint32_t channel, std::weak_ptr<assets::sound> sound, bool looping);
		void set_volume_at_channel(uint32_t channel, float volume_precent);
		void resume_sound_at_channel(uint32_t channel);
		void stop_sound_at_channel(uint32_t channel);
	};
}