#pragma once 
#include "source/assets/sound_asset.h"
#include "include/glm/vec3.hpp"

namespace entities
{
	namespace components
	{
		class listener;
		class sound_emitter;
	}
}

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

		void update();

		void set_audio_rolloff(float rolloff);

		bool is_active_listner(entities::components::listener* listener);
		void set_active_listener(entities::components::listener* active_listener);

		void play_sound(std::weak_ptr<assets::sound> sound);
		void set_volume(float volume_precent);

		void play_sound_at_channel(uint32_t channel, std::weak_ptr<assets::sound> sound, bool looping);
		void set_volume_at_channel(uint32_t channel, float volume_precent);
		void resume_sound_at_channel(uint32_t channel);
		void stop_sound_at_channel(uint32_t channel);
		void set_position_at_channel(uint32_t channel, glm::vec3 position);

		void register_emitter(entities::components::sound_emitter* emitter);
		void unregister_emitter(entities::components::sound_emitter* emitter);
		void emit_sound(entities::components::sound_emitter* emitter, std::weak_ptr<assets::sound> sound, float volume_precent);
	};
}