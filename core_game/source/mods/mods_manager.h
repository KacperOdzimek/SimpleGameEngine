#pragma once
#include "mod.h"

#include <string>
#include <vector>

namespace mods
{
	class mods_manager
	{
	public:
		void load_mod(std::string mod_folder_name);
		void unload_mod();
		std::vector<mods::mod_manifest> get_all_mods_manifests();
	};
}