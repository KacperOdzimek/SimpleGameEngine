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
	/*
		behaviors_manager
		manages lua environement and execution of behaviors
	*/
	class behaviors_manager
	{
	friend assets::behavior;
	friend entities::components::behavior;
	public:
		behaviors_manager();
		~behaviors_manager();

		/*
			call_update_functions
			calls on_update on every registered behavior component
		*/
		void call_update_functions();
		/*
			register_behavior_component
			registered component will be updated every frame
		*/
		void register_behavior_component(entities::components::behavior* comp);
		/*
			unregister_behavior_component
			unregister the component so it is no longer updated
			also calls on_destroy on 
		*/
		void unregister_behavior_component(entities::components::behavior* comp);
	private:
		struct implementation;
		implementation* impl;
		/*
			create_behavior
			loads behavior into lua virtual machine so it can be invoked
			-l-
			[file_path] path to lua file
			[result]	name of the something like namespace containing allocated functions
						required to invoke behavior
		*/
		std::string create_behavior(const std::string& file_path);

		/*
			prepare_call
			informs lua environement about coming behaviors function call
			-l-
			[func]	function to be invoken
			[bhv]	behavior from which function should be called
		*/
		void prepare_call(behaviors::functions func, assets::behavior* bhv);
		/*
			call
			calls behaviors function
			-l-
			[args_amount] amount of function arguments
		*/
		void call(int args_amount);
		/*
			abort
			ends execution of the current lua function
		*/
		void abort();
		/*
			pass_database_ownership
			moves database ownership from variable into the behaviors_manager
		*/
		void pass_database_ownership(std::unique_ptr<behaviors::database>& database);
		/*
			retrieve_database_ownership
			return database ownership from behaviors_manager to the outside
		*/
		std::unique_ptr<behaviors::database> retrieve_database_ownership();
		/*
			destroy_database
			destroys owned database
		*/
		void destroy_database();

		/*
			pass_x_arg
			passes an arg to the behavior function in the following {call} call
		*/
		void pass_pointer_arg(void* arg);
		/*
			pass_x_arg
			passes an arg to the behavior function in the following {call} call
		*/
		void pass_int_arg(uint64_t arg);
		/*
			pass_x_arg
			passes an arg to the behavior function in the following {call} call
		*/
		void pass_float_arg(float arg);
	};
}