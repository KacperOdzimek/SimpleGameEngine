#include "input_manager.h"
#include "source/common/abort.h"
#include "source/assets/input_config_asset.h"

using namespace input;

void input_manager::load_config(std::weak_ptr<assets::input_config> _config_asset)
{
	config_asset = _config_asset.lock();

	action_mappings_states.clear();
	axis_mappings_states.clear();

	for (const auto& action : config_asset->action_mappings)
		action_mappings_states.insert({ action.first, 0 });

	for (const auto& axis : config_asset->axis_mappings)
		action_mappings_states.insert({ axis.first, 0 });
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