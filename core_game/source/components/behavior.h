#pragma once
#include "source/entities/component.h"

#include "source/behaviors/behavior_functions.h"
#include "source/behaviors/behaviors_database.h"

#include <string>

namespace assets
{
	struct behavior;
}

namespace entities
{
	namespace components
	{
		/*
			behavior
			component that links the game logic to the entity
			behavior adds behavior assets to the entity
		*/
		class behavior : virtual public entities::component
		{	
		protected:
			std::shared_ptr<behaviors::database> database;
		public:
			std::shared_ptr<assets::behavior> behavior_asset;
			behavior(uint32_t _id, std::weak_ptr<assets::behavior> _behavior_asset);
			~behavior();
			/*
				call_function
				calls function on the behavior
			*/
			void call_function(behaviors::functions func, std::weak_ptr<entities::entity> other = {});
			/*
				call_custom_function
				calls function of given name on the behavior
			*/
			void call_custom_function(const std::string& name);
			virtual void on_attach() override;
		};
	}
}