#pragma once
#include "asset.h"
#include "glm/vec3.hpp"

namespace assets
{
	struct rendering_config : public asset
	{
		const std::string default_sprite_shader;
		const bool pixel_aligned_camera;
		const glm::vec3 background_color;
		rendering_config(
			bool pixel_aligned_camera,
			std::string default_sprite_shader,
			glm::vec3 background_color
		);
		~rendering_config();
	};
}