#pragma once
#include "collision_config_asset.h"
#include "source/common/abort.h"

namespace assets
{
	physics::collision_preset collision_config::get_preset(uint32_t hashed_name)
	{
		auto itr = collision_presets.find(hashed_name);
		if (itr == collision_presets.end())
			abort("Trying to get non-existent collsion preset");
		return itr->second;
	};
}