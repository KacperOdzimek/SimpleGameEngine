#pragma once
#include "asset.h"

namespace assets
{
	struct rendering_config : public asset
	{
		const std::string default_sprite_shader;
		const bool pixel_aligned_camera;
		rendering_config(
			bool pixel_aligned_camera,
			std::string default_sprite_shader
		);
		~rendering_config();
	};
}