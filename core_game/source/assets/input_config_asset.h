#pragma once
#include "asset.h"
#include "source/input/input_mappings.h"
#include <unordered_map>

namespace input
{
	class input_manager;
}

namespace assets
{
	struct input_config : public asset
	{
		friend input::input_manager;
	private:
		std::unordered_map<std::string, input::action_mapping> action_mappings;
		std::unordered_map<std::string, input::axis_mapping> axis_mappings;
	public:
		input_config(
			std::unordered_map<std::string, input::action_mapping>&& _action_mappings,
			std::unordered_map<std::string, input::axis_mapping>&& _axis_mappings
		) : action_mappings(_action_mappings), axis_mappings(_axis_mappings)
		{
		}
	};
}