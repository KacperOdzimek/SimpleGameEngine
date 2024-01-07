#pragma once
#include <memory>
#include <string>

#include "source/assets/asset.h"

namespace assets
{
	class assets_manager
	{
		struct implementation;
		implementation* impl;
	public:
		assets_manager();
		~assets_manager();
		void set_assets_path(std::string assets_global_path);
		void load_asset_from_json(std::string local_path);
		std::weak_ptr<asset> get_asset(uint32_t hashed_name);
	};
}