#include "input_mappings.h"
#include <algorithm>

using namespace input;

bool action_mapping::get_value(const std::vector<key>& _keys)
{
	return std::find_first_of(keys.begin(), keys.end(),
		_keys.begin(), _keys.end()) != keys.end();
}

float axis_mapping::get_value(const std::vector<key>& _keys)
{
	float value = 0.0f;
	for (auto& key : _keys)
	{
		auto itr = std::find(keys.begin(), keys.end(), key);
		if (itr != keys.end())
			value += itr->value * key.value;
	}		
	return value;
}