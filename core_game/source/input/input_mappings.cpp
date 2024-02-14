#include "input_mappings.h"
#include <algorithm>

using namespace input;

bool action_mapping::get_value(const std::vector<key>& _pressed_key) const
{
	return std::find_first_of(keys.begin(), keys.end(),
		_pressed_key.begin(), _pressed_key.end()) != keys.end();
}

float axis_mapping::get_value(const std::vector<key_state>& _keys) const
{
	float value = 0.0f;
	for (auto& key : _keys)
	{
		auto itr = std::find(keys.begin(), keys.end(), key.key);
		if (itr != keys.end())
			value += itr->axis_value * key.state;
	}		
	return value;
}