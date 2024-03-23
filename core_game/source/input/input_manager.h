#pragma once 
#include "source/assets/input_config_asset.h"
#include "include/glm/vec2.hpp"

namespace input
{
	class input_manager
	{
	private:
		struct implementation;
		implementation* impl;
	public:
		input_manager();
		~input_manager();

		void load_config(std::weak_ptr<assets::input_config> _config_asset);
		void update_mappings_states();

		bool get_action_mapping_value(const std::string& mapping_name);
		bool get_action_mapping_was_just_pressed(const std::string& mapping_name);
		bool get_action_mapping_was_just_relased(const std::string& mapping_name);
		float get_axis_mapping_value(const std::string& mapping_name);
	};
}