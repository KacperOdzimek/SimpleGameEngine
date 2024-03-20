#pragma once
#include "collision_config_asset.h"
#include "source/common/crash.h"

namespace assets
{
	physics::collision_preset collision_config::get_preset(uint32_t hashed_name)
	{
		auto itr = collision_presets.find(hashed_name);
		if (itr == collision_presets.end())
			error_handling::crash(error_handling::error_source::core, "[collision_config::get_preset]",
				"Trying to get non-existent collision preset: " + std::to_string(hashed_name));
		return itr->second;
	};
}