#pragma once
#include "source/entities/component.h"
#include "source/assets/sound_asset.h"

namespace entities
{
	namespace components
	{
		/*
			sound_emitter
			plays sound in owners position
		*/
		class sound_emitter : virtual public entities::component
		{
		public:
			sound_emitter(uint32_t id);
			~sound_emitter();
			virtual void on_attach() override;
			void emit_sound(std::weak_ptr<assets::sound> sound, float volume_precent);
		};
	}
}