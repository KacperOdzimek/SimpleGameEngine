#pragma once
#include <string>
#include <vector>

namespace mods
{
	class mods_manager
	{
	public:
		void load_mod(std::string mod_name);
		void load_mod_selection_mod();
		void unload_mod();
		std::string get_current_mod_name();
		std::string get_mods_directory();
		std::vector<std::string> get_all_mods();
	};
}