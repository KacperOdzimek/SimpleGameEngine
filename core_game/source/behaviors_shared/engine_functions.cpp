#include "engine_functions.h"

#include "source/behaviors_shared/utilities.h"

#include "source/common/common.h"
#include "source/entities/world.h"
#include "source/entities/entity.h"

#include "source/filesystem/filesystem.h"

#include "include/nlohmann/json.hpp"

//TODO REMOVE
#include "source/components/sprite.h"

namespace behaviors
{
	namespace lua_shared
	{
		namespace engine
		{
			int _en_load_scene(lua_State* L)
			{
				auto name = load_id(L, 1, "[_en_load_scene]", "Scene");
				auto scene_asset = load_asset_path(L, 2, "[_en_load_scene]");

				common::world->create_scene(
					name,
					assets::cast_asset<assets::scene>(common::assets_manager->safe_get_asset(scene_asset))
				);

				return 0;
			}

			int _en_unload_scene(lua_State* L)
			{
				auto name = load_id(L, 1, "[_en_load_scene]", "Scene");

				common::world->remove_scene(name);

				return 0;
			}

			int _en_create_entities_from_tilemap(lua_State* L)
			{
				auto tilemap_asset = load_asset_path(L, 1, "[_en_create_entities_from_tilemap]");
				auto creator_function = lua_tocfunction(L, 2);

				auto header_file = filesystem::load_file(tilemap_asset + ".json");
				auto header = nlohmann::json::parse(header_file);
				header_file.close();

				if (!(header.contains("path") && header.at("path").is_string()))
					error_handling::crash(error_handling::error_source::core, "[_en_create_entities_from_tilemap]",
						"Missing tilemap path / tilemap path isnt string");

				auto tilemap_file_path = filesystem::get_package(tilemap_asset) + std::string(header.at("path"));
				
				auto tilemap_file = filesystem::load_file(tilemap_file_path);
				auto tilemap = nlohmann::json::parse(tilemap_file);
				tilemap_file.close();

				if (!(tilemap.contains("layers") && tilemap.at("layers").is_array()))
					error_handling::crash(error_handling::error_source::core, "[_en_create_entities_from_tilemap]",
						"Source file is missing layers");

				if (!(tilemap.contains("width") && tilemap.at("width").is_number_integer()))
					error_handling::crash(error_handling::error_source::core, "[_en_create_entities_from_tilemap]",
						"Source file is missing width");
				unsigned int width = tilemap.at("width");

				if (!(tilemap.contains("height") && tilemap.at("height").is_number_integer()))
					error_handling::crash(error_handling::error_source::core, "[_en_create_entities_from_tilemap]",
						"Source file is missing height");
				unsigned int height = tilemap.at("height");

				for (auto& layer : tilemap.at("layers"))
				{
					if (!layer.contains("objects"))
						continue;

					auto& objects = layer.at("objects");

					for (auto& object : objects)
					{
						float x = object.at("x");
						float y = object.at("y");

						auto e = new ::entities::entity{};
						
						auto sprite = new ::entities::components::sprite{
						12,
						::assets::cast_asset<::assets::texture>(::common::assets_manager->safe_get_asset("mod/debug/test_unit_flip")),
						12
						};
						
						auto ax = x / common::pixels_per_world_unit - float(width) / 4.0f;
						auto ay = -(y / common::pixels_per_world_unit - float(height) / 4.0f) - 0.5;
						e->attach_component(sprite);
						e->teleport({ ax, ay });
						e->layer = 4;
					}
				}

				return 0;
			}

			void register_shared(lua_State* L)
			{
				lua_register(L, "_en_load_scene", _en_load_scene);
				lua_register(L, "_en_unload_scene", _en_unload_scene);
				lua_register(L, "_en_create_entities_from_tilemap", _en_create_entities_from_tilemap);
			}
		}
	}
}