#pragma once
#include "source/entities/component.h"

namespace entities
{
	namespace components
	{
		/*
			listener
			represents listener in audio subsystem
		*/
		class listener : virtual public entities::component
		{
		public:
			listener(uint32_t id);
			~listener();
			virtual void on_attach() override;
		private:
		};
	}
}