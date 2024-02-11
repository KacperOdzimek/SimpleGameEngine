#pragma once 
#include "key.h"
#include <vector>

namespace input
{
	struct action_mapping
	{
		std::vector<key> keys;
		bool get_value(const std::vector<key>& keys);
	};

	struct axis_mapping
	{
		std::vector<key> keys;
		float get_value(const std::vector<key>& keys);
	};
}