#pragma once

#include "asset.h"
#include "source/physics/collision.h"

#include <map>

namespace assets
{
	struct collision_config : public asset
	{
		/*
		body_types
		hashed body type to it's index in collision_preset
		*/
		const std::map<uint32_t, uint8_t> body_types;
		/*
		collsion_presets
		contains predefinied presets
		-l-
		key is hashed preset name
		preset is preset
		*/
		const std::map<uint32_t, physics::collision_preset> collision_presets;

		collision_config(
			std::map<uint32_t, uint8_t>& _body_types,
			std::map<uint32_t, physics::collision_preset>& _collision_presets
			) : body_types(std::move(_body_types)), collision_presets(std::move(_collision_presets))
		{
		}

		physics::collision_preset get_preset(uint32_t hashed_name)
		{
			auto itr = collision_presets.find(hashed_name);
			if (itr == collision_presets.end())
				return 0;
			return itr->second;
		};
	};
}