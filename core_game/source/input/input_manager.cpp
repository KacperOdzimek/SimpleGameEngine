#include "input_manager.h"
#include "source/common/crash.h"

#include "source/assets/input_config_asset.h"

#include "source/common/common.h"
#include "source/window/window_manager.h"

#include <string>
#include <unordered_map>
#include <set>

using namespace input;

struct input_manager::implementation
{
	bool a_action_state_is_current = true;
	struct action_mapping_state
	{
		bool a = false;
		bool b = false;
	};
	std::unordered_map<std::string, action_mapping_state> action_mappings_states;
	std::unordered_map<std::string, float> axis_mappings_states;
	std::shared_ptr<assets::input_config> config_asset;
	std::set<input::key> keys_to_check;
};

input_manager::input_manager()
{
	impl = new implementation;
}

input_manager::~input_manager()
{
	delete impl;
}

void input_manager::load_config(std::weak_ptr<assets::input_config> _config_asset)
{
	impl->config_asset = _config_asset.lock();

	impl->a_action_state_is_current = false;

	impl->action_mappings_states.clear();
	impl->axis_mappings_states.clear();

	for (const auto& action : impl->config_asset->action_mappings)
	{
		impl->action_mappings_states.insert({ action.first, {} });
		for (auto& key : action.second.keys)
			impl->keys_to_check.insert(key);
	}

	for (const auto& axis : impl->config_asset->axis_mappings)
	{
		impl->axis_mappings_states.insert({ axis.first, 0.0f });
		for (auto& key : axis.second.keys)
			impl->keys_to_check.insert(key);
	}
}

void input_manager::update_mappings_states()
{
	impl->a_action_state_is_current = !impl->a_action_state_is_current;

	std::vector<key_state> states;
	for (auto& k : impl->keys_to_check)
	{
		switch (k.key_type)
		{
		case input::key_type::keyboard: case input::key_type::mouse:
			states.push_back(common::window_manager->get_key_state(k));
			break;
		case input::key_type::gamepad:
			//TODO
			break;
		}
	}
	
	std::vector<key> pressed_keys;
	for (auto& k : states)
		if (k.state == 1)
			pressed_keys.push_back(k.key);

	for (const auto& action : impl->config_asset->action_mappings)
	{
		auto& state = impl->action_mappings_states.at(action.first);
		if (impl->a_action_state_is_current)
			state.a = action.second.get_value(pressed_keys);
		else
			state.b = action.second.get_value(pressed_keys);
	}

	for (const auto& axis : impl->config_asset->axis_mappings)
		impl->axis_mappings_states.at(axis.first) = axis.second.get_value(states);
}

bool input_manager::get_action_mapping_value(const std::string& mapping_name)
{
	auto itr = impl->action_mappings_states.find(mapping_name);
	if (itr == impl->action_mappings_states.end())
		error_handling::crash(error_handling::error_source::core, "[input_manager::get_action_mapping_value]", 
			"Trying to get value of non-existent action mapping: " + mapping_name);
	if (impl->a_action_state_is_current)
		return itr->second.a;
	return itr->second.b;
}

bool input_manager::get_action_mapping_was_just_pressed(const std::string& mapping_name)
{
	auto itr = impl->action_mappings_states.find(mapping_name);
	if (itr == impl->action_mappings_states.end())
		error_handling::crash(error_handling::error_source::core, "[input_manager::get_action_mapping_was_just_pressed]",
			"Trying to get value of non-existent action mapping: " + mapping_name);
	if (impl->a_action_state_is_current)
		return (itr->second.a && !itr->second.b);
	return (itr->second.b && !itr->second.a);
}

bool input_manager::get_action_mapping_was_just_relased(const std::string& mapping_name)
{
	auto itr = impl->action_mappings_states.find(mapping_name);
	if (itr == impl->action_mappings_states.end())
		error_handling::crash(error_handling::error_source::core, "[input_manager::get_action_mapping_was_just_relased]",
			"Trying to get value of non-existent action mapping: " + mapping_name);
	if (impl->a_action_state_is_current)
		return (!itr->second.a && itr->second.b);
	return (!itr->second.b && itr->second.a);
}

float input_manager::get_axis_mapping_value(const std::string& mapping_name)
{
	auto itr = impl->axis_mappings_states.find(mapping_name);
	if (itr == impl->axis_mappings_states.end())
		error_handling::crash(error_handling::error_source::core, "[input_manager::get_axis_mapping_value]",
			"Trying to get value of non-existent axis mapping: " + mapping_name);
	return itr->second;
}