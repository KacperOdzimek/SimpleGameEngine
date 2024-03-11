#pragma once
#include "rendering_config_asset.h"
#include "source/common/crash.h"

#include "source/utilities/hash_string.h"

#include "source/common/common.h"
#include "source/assets/assets_manager.h"

namespace assets
{
	rendering_config::rendering_config(
		std::string _default_sprite_shader
	)
		: default_sprite_shader(_default_sprite_shader)
	{
	}

	rendering_config::~rendering_config()
	{
	}
}