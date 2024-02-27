#include "behavior.h"
#include "source/common/common.h"
#include "source/behaviors/behaviors_manager.h"
#include "source/entities/world.h"

entities::components::behavior::behavior(uint32_t _id, std::weak_ptr<assets::behavior> _behavior_asset) 
	: component(_id), behavior_asset(_behavior_asset.lock()), database(std::make_unique<behaviors::database>())
{
}

entities::components::behavior::~behavior()
{
	call_function(behaviors::functions::destroy);
	common::behaviors_manager->unregister_behavior_component(this);
}

void entities::components::behavior::on_attach()
{
	common::behaviors_manager->register_behavior_component(this);
	call_function(behaviors::functions::init);
}

void entities::components::behavior::call_function(behaviors::functions func, std::weak_ptr<entities::entity> other)
{
	bool implemented = common::behaviors_manager->prepare_behavior_function_call(func, behavior_asset.get());
	if (!implemented)
		return;

	auto owner_weak_ptr = get_owner_weak();
	common::behaviors_manager->create_frame(database, common::world->get_active_scene());

	if (!owner_weak_ptr.expired() && behavior_asset)
		switch (func)
		{
		case behaviors::functions::init:
			common::behaviors_manager->pass_int_arg((uint64_t)&owner_weak_ptr);
			common::behaviors_manager->call(1);
			break;
		case behaviors::functions::update:
			common::behaviors_manager->prepare_behavior_function_call(func, behavior_asset.get());
			common::behaviors_manager->pass_int_arg((uint64_t)&owner_weak_ptr);
			common::behaviors_manager->pass_float_arg(common::delta_time);
			common::behaviors_manager->call(2);
			break;
		case behaviors::functions::destroy:
			common::behaviors_manager->prepare_behavior_function_call(func, behavior_asset.get());
			common::behaviors_manager->pass_int_arg((uint64_t)&owner_weak_ptr);
			common::behaviors_manager->call(1);
			break;
		case behaviors::functions::on_overlap:
			common::behaviors_manager->prepare_behavior_function_call(func, behavior_asset.get());
			common::behaviors_manager->pass_int_arg((uint64_t)&owner_weak_ptr);
			common::behaviors_manager->pass_int_arg((uint64_t)&other);
			common::behaviors_manager->call(2);
			break;
		case behaviors::functions::on_collide:
			common::behaviors_manager->prepare_behavior_function_call(func, behavior_asset.get());
			common::behaviors_manager->pass_int_arg((uint64_t)&owner_weak_ptr);
			common::behaviors_manager->pass_int_arg((uint64_t)&other);
			common::behaviors_manager->call(2);
			break;
		}
	common::behaviors_manager->pop_frame();
}

void entities::components::behavior::call_custom_function(const std::string& name)
{
	auto owner_weak_ptr = get_owner_weak();
	common::behaviors_manager->create_frame(database, common::world->get_active_scene());
	common::behaviors_manager->prepare_custom_behavior_function_call(name, this->behavior_asset.get());
	common::behaviors_manager->call(2);
	common::behaviors_manager->pop_frame();
}