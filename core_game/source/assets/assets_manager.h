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

		void load_asset(std::string local_path);
		std::weak_ptr<asset> get_asset(uint32_t hashed_name);
		std::weak_ptr<asset> safe_get_asset(std::string path);

		void lock_asset(uint32_t hashed_name);
		void unlock_asset(uint32_t hashed_name);

		void unload_unreferenced_assets();
	};
}