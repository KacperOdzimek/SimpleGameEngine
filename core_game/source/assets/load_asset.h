#pragma once
#include "asset.h"
#include <memory>
#include "nlohmann/json.hpp"

namespace assets
{
	namespace loading
	{
		struct load_data
		{
			const nlohmann::json* header_data;
			std::string package;
			std::string header_folder;
		};

		std::shared_ptr<asset> load_texture(const load_data& data);
		std::shared_ptr<asset> load_sprite_sheet(const load_data& data);
		std::shared_ptr<asset> load_flipbook(const load_data& data);
		std::shared_ptr<asset> load_tileset(const load_data& data);
		std::shared_ptr<asset> load_tilemap(const load_data& data);
		std::shared_ptr<asset> load_shader(const load_data& data);
		std::shared_ptr<asset> load_sound(const load_data& data);
		std::shared_ptr<asset> load_behavior(const load_data& data);
		std::shared_ptr<asset> load_scene(const load_data& data);
		std::shared_ptr<asset> load_mesh(const load_data& data);
		std::shared_ptr<asset> load_rendering_config(const load_data& data);
		std::shared_ptr<asset> load_input_config(const load_data& data);
		std::shared_ptr<asset> load_collision_config(const load_data& data);
	}
}