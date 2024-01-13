#pragma once
#include "asset.h"
#include <memory>
#include "nlohmann/json.hpp"

namespace assets
{
	namespace loading
	{
		std::shared_ptr<asset> load_texture(std::string& assets_folder_path, nlohmann::json& data);
		std::shared_ptr<asset> load_shader(std::string& assets_folder_path, nlohmann::json& data);
		std::shared_ptr<asset> load_behavior(std::string& assets_folder_path, nlohmann::json& data);
	}
}