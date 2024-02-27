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
	class scene;

	namespace components
	{
		struct behavior;
	}
}

namespace behaviors
{
	struct frame;
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
		/*
			get_current_frame
			returns current frame
		*/
		const behaviors::frame* get_current_frame();
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
			informs lua environement about coming behavior function call
			returns true if function can be called
			returns false if function is not implemented and futher actions should be abort
			-l-
			[func]	function to be invoken
			[bhv]	behavior from which function should be called
		*/
		bool prepare_call(behaviors::functions func, assets::behavior* bhv);
		/*
			prepare_custom_call
			informs lua environement about coming behavior function call
			returns true if function can be called
			returns false if function is not implemented and futher actions should be abort
			-l-
			[func_name]	name of function to be invoken
			[bhv]		behavior from which function should be called
		*/
		bool prepare_custom_call(const std::string& name, assets::behavior* bhv);
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
		/*
			create_frame
			creates frame that holds program state
			-l-
			required before call
		*/
		void create_frame(std::shared_ptr<database> database, std::unique_ptr<entities::scene>* scene_context);
		/*
			pop_frame
			pops frame from the top of the stack
			-l-
			required after call
		*/
		void pop_frame();
	};
}