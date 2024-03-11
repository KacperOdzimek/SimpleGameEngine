#pragma once
#include "asset.h"

namespace assets
{
	struct rendering_config : public asset
	{
		const std::string default_sprite_shader;
		rendering_config(
			std::string default_sprite_shader
		);
		~rendering_config();
	};
}