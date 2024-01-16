#include "behavior.h"
#include "source/common/common.h"
#include "source/behaviors/behaviors_manager.h"

entities::components::behavior::behavior(uint32_t _id, std::weak_ptr<assets::behavior> _behavior_asset) 
	: component(_id), behavior_asset(_behavior_asset), database(std::make_unique<behaviors::database>())
{
}

entities::components::behavior::~behavior()
{
	common::behaviors_manager->unregister_behavior_component(this);
}

void entities::components::behavior::on_attach()
{
	common::behaviors_manager->register_behavior_component(this);
}

void entities::components::behavior::call_function(behaviors::functions func)
{
	auto owner_weak_ptr = get_owner_weak();
	if (!owner_weak_ptr.expired() && !behavior_asset.expired())
		switch (func)
		{
		case behaviors::functions::init:
			common::behaviors_manager->pass_database_ownership(database);
			common::behaviors_manager->prepare_call(func, behavior_asset.lock().get());
			common::behaviors_manager->pass_int_arg((uint64_t)&owner_weak_ptr);
			common::behaviors_manager->call();
			break;
		case behaviors::functions::update:
			common::behaviors_manager->pass_database_ownership(database);
			common::behaviors_manager->prepare_call(func, behavior_asset.lock().get());
			common::behaviors_manager->pass_int_arg((uint64_t)&owner_weak_ptr);
			common::behaviors_manager->pass_float_arg(common::delta_time);
			common::behaviors_manager->call();
			break;
		case behaviors::functions::destroy:
			if (database.get() != nullptr)
				common::behaviors_manager->pass_database_ownership(database);
			common::behaviors_manager->prepare_call(func, behavior_asset.lock().get());
			common::behaviors_manager->pass_int_arg((uint64_t)&owner_weak_ptr);
			common::behaviors_manager->call();
			return;	//as destroy may be called during update execution we don't want to purge the database
		}

	if (!owner_weak_ptr.expired())
	{
		database = common::behaviors_manager->retrieve_database_ownership();
	}
	else
		common::behaviors_manager->destroy_database();
}