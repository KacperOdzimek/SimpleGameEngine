#pragma once 
#include "key.h"
#include <vector>

namespace input
{
	struct action_mapping
	{
		std::vector<key> keys;
		bool get_value(const std::vector<key>& pressed_keys) const;
	};

	struct axis_mapping
	{
		std::vector<key> keys;
		float get_value(const std::vector<key_state>& keys) const;
	};
}