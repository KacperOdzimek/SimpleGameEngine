#pragma once 
#include "source/assets/input_config_asset.h"

#include "include/glm/vec2.hpp"

#include <string>
#include <unordered_map>
#include <set>

namespace input
{
	class input_manager
	{
	private:
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
	public:
		void load_config(std::weak_ptr<assets::input_config> _config_asset);
		void update_mappings_states();

		bool get_action_mapping_value(const std::string& mapping_name);
		bool get_action_mapping_was_just_pressed(const std::string& mapping_name);
		bool get_action_mapping_was_just_relased(const std::string& mapping_name);
		float get_axis_mapping_value(const std::string& mapping_name);
	};
}