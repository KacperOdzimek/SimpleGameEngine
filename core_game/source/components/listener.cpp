#include "listener.h"

#include "source/common/common.h"
#include "source/audio/audio_manager.h"

namespace entities
{
	namespace components
	{
		listener::listener(uint32_t _id) : component(_id) {};

		listener::~listener()
		{
			if (common::audio_manager->is_active_listner(this))
				common::audio_manager->set_active_listener(nullptr);
		}

		void listener::on_attach()
		{
		}
	}
}