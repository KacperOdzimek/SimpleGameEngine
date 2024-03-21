#include "sound_emitter.h"

#include "source/common/common.h"
#include "source/audio/audio_manager.h"

namespace entities
{
	namespace components
	{
		sound_emitter::sound_emitter(uint32_t _id) : component(_id) {};

		void sound_emitter::on_attach()
		{
			common::audio_manager->register_emitter(this);
		}

		sound_emitter::~sound_emitter() 
		{
			common::audio_manager->unregister_emitter(this);
		}

		void sound_emitter::emit_sound(std::weak_ptr<assets::sound> sound)
		{

		}
	}
}