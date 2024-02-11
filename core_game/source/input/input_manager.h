#pragma once 
#include "include/glm/vec2.hpp"
#include <string>
#include <unordered_map>

namespace input
{
	class input_manager
	{
	private:
		std::unordered_map<std::string, bool> action_mappings_states;
		std::unordered_map<std::string, float> axis_mappings_states;
	public:
		void reload_config();
		void update_mappings_states();

		bool get_action_mapping_value(const std::string& mapping_name);
		float get_axis_mapping_value(const std::string& mapping_name);
		//glm::vec2 get_mouse_position_world_space();
		//glm::vec2 geo_mouse_position_screen_space();
	};
}