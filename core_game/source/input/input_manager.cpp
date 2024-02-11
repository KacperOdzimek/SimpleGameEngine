#include "input_manager.h"
#include "source/common/abort.h"

using namespace input;

void input_manager::reload_config()
{

}

void input_manager::update_mappings_states()
{

}

bool input_manager::get_action_mapping_value(const std::string& mapping_name)
{
	auto itr = action_mappings_states.find(mapping_name);
	if (itr == action_mappings_states.end())
		abort("Trying to get value of non-existent action mapping");
	return itr->second;
}

float input_manager::get_axis_mapping_value(const std::string& mapping_name)
{
	auto itr = axis_mappings_states.find(mapping_name);
	if (itr == axis_mappings_states.end())
		abort("Trying to get value of non-existent action mapping");
	return itr->second;
}