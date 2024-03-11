#include "mods_manager.h"

#include "include/nlohmann/json.hpp"
#include "source/filesystem/filesystem.h"

#include "source/common/crash.h"

#include "source/common/common.h"
#include "source/assets/assets_manager.h"
#include "source/entities/world.h"
#include "source/input/input_manager.h"

#include "source/utilities/hash_string.h"

void mods::mods_manager::load_mod(std::string mod_folder_name)
{
	filesystem::set_mod_assets_directory(mod_folder_name);
	auto manifest_file = filesystem::load_file("mod/manifest.json");
	nlohmann::json manifest = nlohmann::json::parse(manifest_file);
	manifest_file.close();

	if (!(manifest.contains("start_scene") && manifest.at("start_scene").is_string()))
		error_handling::crash(error_handling::error_source::core, "[mods_manager::load_mod]",
			"Invalid mod manifest: missing start_scene / start_scene isn't string");

	std::string start_scene = manifest.at("start_scene");

	if (!(manifest.contains("pixels_per_unit") && manifest.at("pixels_per_unit").is_number()))
		error_handling::crash(error_handling::error_source::core, "[mods_manager::load_mod]",
			"Invalid mod manifest: missing pixels_per_unit / pixels_per_unit isn't number");

	common::pixels_per_world_unit = manifest.at("pixels_per_unit");

	common::assets_manager->load_asset("mod/collision_config");
	common::assets_manager->lock_asset(utilities::hash_string("mod/collision_config"));

	common::assets_manager->load_asset("mod/input_config");
	common::assets_manager->lock_asset(utilities::hash_string("mod/input_config"));

	common::assets_manager->load_asset("mod/rendering_config");
	common::assets_manager->lock_asset(utilities::hash_string("mod/rendering_config"));

	auto input_config = assets::cast_asset<assets::input_config>
		(common::assets_manager->get_asset(utilities::hash_string("mod/input_config")));
	common::input_mananger->load_config(input_config.lock());

	common::world->create_scene(
		utilities::hash_string("start_scene"),
		assets::cast_asset<assets::scene>(common::assets_manager->safe_get_asset("mod" + start_scene))
	);
}

void mods::mods_manager::unload_mod()
{
	common::assets_manager->unlock_asset(utilities::hash_string("mod/input_config"));
	common::assets_manager->unlock_asset(utilities::hash_string("mod/collision_config"));
	common::assets_manager->unlock_asset(utilities::hash_string("mod/rendering_config"));
}

std::vector<mods::mod_manifest> mods::mods_manager::get_all_mods_manifests()
{
	return {};
}