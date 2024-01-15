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
	if (!behavior_asset.expired())
		switch (func)
		{
		case behaviors::functions::init:
			common::behaviors_manager->prepare_call(func, behavior_asset.lock().get(), database.get());
			common::behaviors_manager->pass_int_arg((uint64_t)get_owner_weak());
			common::behaviors_manager->call();
			break;
		case behaviors::functions::update:
			common::behaviors_manager->prepare_call(func, behavior_asset.lock().get(), database.get());
			common::behaviors_manager->pass_int_arg((uint64_t)get_owner_weak());
			common::behaviors_manager->pass_float_arg(common::delta_time);
			common::behaviors_manager->call();
			break;
		case behaviors::functions::destroy:
			common::behaviors_manager->prepare_call(func, behavior_asset.lock().get(), database.get());
			common::behaviors_manager->pass_int_arg((uint64_t)get_owner_weak());
			common::behaviors_manager->call();
			break;
		}
}