#pragma once
#include "source/common/common.h"
#include "source/entities/component.h"

#include "source/behaviors/behavior_functions.h"
#include "source/behaviors/behaviors_database.h"

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
		protected:
			std::shared_ptr<behaviors::database> database;
		public:
			std::shared_ptr<assets::behavior> behavior_asset;
			behavior(uint32_t _id, std::weak_ptr<assets::behavior> _behavior_asset);
			~behavior();
			void call_function(behaviors::functions func, std::weak_ptr<entities::entity> other = {});
			void call_custom_function(const std::string& name);
			virtual void on_attach();
		};
	}
}