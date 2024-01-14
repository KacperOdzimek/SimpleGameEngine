#pragma once
#include "source/common/common.h"
#include "source/entities/component.h"

#include "source/behaviors/behavior_functions.h"

namespace assets
{
	struct behavior;
}

namespace entities
{
	namespace components
	{
		class behavior : public entities::component
		{	
		public:
			std::weak_ptr<assets::behavior> behavior_asset;
			behavior(uint32_t _id, std::weak_ptr<assets::behavior> _behavior_asset);
			~behavior();
			void call_function(behaviors::functions func);
			virtual void on_attach();
		};
	}
}