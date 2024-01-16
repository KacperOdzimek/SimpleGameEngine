#pragma once
#include <string>
#include "behavior_functions.h"
#include "behaviors_database.h"

namespace assets
{
	struct behavior;
}

namespace entities
{
	namespace components
	{
		struct behavior;
	}
}

class lua_State;

namespace behaviors
{
	class behaviors_manager
	{
	friend assets::behavior;
	friend entities::components::behavior;
	public:
		behaviors_manager();
		~behaviors_manager();

		void call_update_functions();
		void register_behavior_component(entities::components::behavior* comp);
		void unregister_behavior_component(entities::components::behavior* comp);
	private:
		struct implementation;
		implementation* impl;
		std::string create_behavior(const std::string& file_path);

		void prepare_call(behaviors::functions func, assets::behavior* bhv);
		void call();

		void pass_database_ownership(std::unique_ptr<behaviors::database>& database);
		std::unique_ptr<behaviors::database> retrieve_database_ownership();
		void destroy_database();

		void pass_pointer_arg(void* arg);
		void pass_int_arg(uint64_t arg);
		void pass_float_arg(float arg);
	};
}