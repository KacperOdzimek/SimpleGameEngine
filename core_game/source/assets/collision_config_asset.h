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
		const std::map<uint32_t, std::string> collision_presets_names;

		collision_config(
			std::map<uint32_t, uint8_t>& _body_types,
			std::map<uint32_t, physics::collision_preset>& _collision_presets,
			std::map<uint32_t, std::string>& _collision_presets_names
			) : body_types(std::move(_body_types)), collision_presets(std::move(_collision_presets)), 
				collision_presets_names(std::move(_collision_presets_names))
		{
		}

		physics::collision_preset get_preset(uint32_t hashed_name);
	};
}