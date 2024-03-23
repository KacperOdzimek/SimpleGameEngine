#include "engine_functions.h"

#include "source/behaviors_shared/utilities.h"

#include "source/common/common.h"
#include "source/rendering/renderer.h"

#include "source/entities/world.h"
#include "source/entities/entity.h"

#include "source/components/camera.h"

#include "source/filesystem/filesystem.h"

#include "include/nlohmann/json.hpp"

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

			void push_number_to_table(lua_State* L, const char* name, float value)
			{
				lua_pushstring(L, name);
				lua_pushnumber(L, value);
				lua_settable(L, -3);
			}

			void push_string_to_table(lua_State* L, const char* name, const char* value)
			{
				lua_pushstring(L, name);
				lua_pushstring(L, value);
				lua_settable(L, -3);
			}

			std::string create_path(const std::string& path, const std::string& package)
			{
				if (path.at(0) == '$')
					return path;
				return package + path;
			}

			int _en_create_entities_from_tilemap(lua_State* L)
			{
				auto tilemap_asset = load_asset_path(L, 1, "[_en_create_entities_from_tilemap]");

				int creator_function_ref = luaL_ref(L, LUA_REGISTRYINDEX);	//Save creator function in registry
																			//so we can use it multiple times

				filesystem::set_active_assets_directory(filesystem::get_owning_folder(tilemap_asset));
				filesystem::set_active_assets_directory_enabled(true);

				auto header_file = filesystem::load_file(tilemap_asset + ".json");
				auto header = nlohmann::json::parse(header_file);
				header_file.close();

				if (!(header.contains("path") && header.at("path").is_string()))
					error_handling::crash(error_handling::error_source::core, "[_en_create_entities_from_tilemap]",
						"Missing tilemap path / tilemap path isnt string");

				auto tilemap_file_path = create_path(std::string(header.at("path")), filesystem::get_package(tilemap_asset));
				
				auto tilemap_file = filesystem::load_file(tilemap_file_path);
				auto tilemap = nlohmann::json::parse(tilemap_file);
				tilemap_file.close();

				filesystem::set_active_assets_directory_enabled(false);

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

				int layers_counter = -1;
				int object_layers_counter = -1;
				for (auto& layer : tilemap.at("layers"))
				{
					layers_counter++;

					if (!layer.contains("objects"))
						continue;

					object_layers_counter++;

					auto& objects = layer.at("objects");

					for (auto& object : objects)
					{
						if (object.at("visible") == false)
							continue;							//Discard invisible objects

						float x = object.at("x");
						float y = object.at("y");

						std::weak_ptr<::entities::entity> e = (new ::entities::entity{})->get_weak();

						//scene space location
						float sx = static_cast<float>(x / common::pixels_per_world_unit - float(width) / 4.0f);
						float sy = static_cast<float>(-(y / common::pixels_per_world_unit - float(height) / 4.0f) - 0.5);

						e.lock()->teleport({ sx, sy });
						e.lock()->layer = layers_counter;

						lua_rawgeti(L, LUA_REGISTRYINDEX, creator_function_ref);	//Restore the creator from registry

						lua_createtable(L, 0, 0);

						lua_pushstring(L, "entity");
						push_entity(L, e);
						lua_settable(L, -3);

						push_string_to_table(L, "name", std::string(object.at("name")).c_str());
						push_string_to_table(L, "class", std::string(object.at("type")).c_str());

						push_number_to_table(L, "x", sx);
						push_number_to_table(L, "y", sy);
						push_number_to_table(L, "layer", static_cast<float>(layers_counter));
						push_number_to_table(L, "object_layer", static_cast<float>(object_layers_counter));

						if (object.contains("width"))
							push_number_to_table(L, "width", 
								static_cast<float>(float(object.at("width")) / common::pixels_per_world_unit)
							);

						if (object.contains("height"))
							push_number_to_table(L, "height",
								static_cast<float>(float(object.at("height")) / common::pixels_per_world_unit)
							);

						if (object.contains("properties") && object.at("properties").is_array())
							for (auto& prop : object.at("properties"))
							{
								lua_pushstring(L, std::string(prop.at("name")).c_str());
								std::string type = prop.at("type");
								auto& value = prop.at("value");

								if (type == "int")
									lua_pushinteger(L, int(value));
								else if (type == "float")
									lua_pushnumber(L, float(value));
								else if (type == "bool")
									lua_pushboolean(L, bool(value));
								else if (type == "string")
									lua_pushstring(L, std::string(value).c_str());
								else
									error_handling::crash(error_handling::error_source::core, "[_en_create_entities_from_tilemap]",
										"Unsuported property type: " + type);

								lua_settable(L, -3);
							}

						auto err = lua_pcall(L, 1, 0, 0);
						if (err != LUA_OK)
							error_handling::crash(error_handling::error_source::core, "[_en_create_entities_from_tilemap]", 
								lua_tostring(L, -1));
					}
				}

				return 0;
			}

			int _en_viewport_to_world(lua_State* L)
			{
				float x = static_cast<float>(lua_tonumber(L, 1));
				float y = static_cast<float>(lua_tonumber(L, 2));

				auto cam = common::renderer->get_active_camera();
				auto cam_loc = cam->get_owner_weak().lock()->get_location();

				float wx, wy;

				wx = cam_loc.x + (x * cam->ortho_width / 2);
				wy = cam_loc.y + (y * (cam->ortho_width * (9.0f / 16.0f) / 2));

				lua_pushnumber(L, wx);
				lua_pushnumber(L, wy);

				return 2;
			}

			void register_shared(lua_State* L)
			{
				lua_register(L, "_en_load_scene", _en_load_scene);
				lua_register(L, "_en_unload_scene", _en_unload_scene);
				lua_register(L, "_en_create_entities_from_tilemap", _en_create_entities_from_tilemap);
				lua_register(L, "_en_viewport_to_world", _en_viewport_to_world);
			}
		}
	}
}