#include "input_mappings.h"
#include <algorithm>

using namespace input;

bool action_mapping::get_value(const std::vector<key_state>& _keys) const
{
	std::vector<key> __keys;
	for (auto& k : _keys)
		__keys.push_back(k.key);

	return std::find_first_of(keys.begin(), keys.end(),
		__keys.begin(), __keys.end()) != keys.end();
}

float axis_mapping::get_value(const std::vector<key_state>& _keys) const
{
	float value = 0.0f;
	for (auto& key : _keys)
	{
		auto itr = std::find(keys.begin(), keys.end(), key.key);
		if (itr != keys.end())
			value += itr->axis_value * key.key.axis_value;
	}		
	return value;
}