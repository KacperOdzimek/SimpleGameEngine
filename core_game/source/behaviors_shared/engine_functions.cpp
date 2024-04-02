#include "engine_functions.h"

#include "source/behaviors_shared/utilities.h"

#include "source/common/common.h"
#include "source/rendering/renderer.h"
#include "source/mods/mods_manager.h"
#include "source/filesystem/filesystem.h"
#include "source/behaviors/behaviors_manager.h"

#include "source/behaviors/frame.h"

#include "source/entities/world.h"
#include "source/entities/entity.h"

#include "source/components/camera.h"

#include "source/assets/custom_data_assset.h"

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
				float x_offset = static_cast<float>(lua_tonumber(L, 3));
				float y_offset = static_cast<float>(lua_tonumber(L, 4));

				common::world->create_scene(
					name,
					{ x_offset , y_offset },
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

			int _en_get_entities_in_scene(lua_State* L)
			{
				auto name = load_id(L, 1, "[_en_get_all_entities_in_scene]", "Scene");
				
				auto scene = common::world->get_scene(name);

				lua_newtable(L);
				int counter = 1;
				for (auto& entity : scene->entities)
				{
					lua_pushinteger(L, counter);
					push_entity(L, entity);
					lua_settable(L, -3);
					counter++;
				}

				return 1;
			}

			std::string create_path(const std::string& path, const std::string& package)
			{
				if (path.at(0) == '$')
					return path;
				return package + path;
			}

			nlohmann::json load_template(std::string path)
			{
				filesystem::set_active_assets_directory_enabled(true);
				auto file = filesystem::load_file(path);
				filesystem::set_active_assets_directory_enabled(false);

				auto _template = nlohmann::json::parse(file);

				if (!(_template.contains("object") && _template.at("object").is_object()))
					error_handling::crash(error_handling::error_source::core, "[_en_create_entities_from_tilemap]",
						"Invalid template: " + path);

				return _template.at("object");
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

				std::string tilemap_folder = filesystem::get_owning_folder(tilemap_file_path) + "/";

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
						float x = object.at("x");
						float y = object.at("y");

						std::weak_ptr<::entities::entity> e = (new ::entities::entity{ common::behaviors_manager->get_current_frame()->scene_context })->get_weak();

						//scene space location
						float sx = static_cast<float>(x / common::pixels_per_world_unit - float(width) / 4.0f);
						float sy = static_cast<float>(-(y / common::pixels_per_world_unit - float(height) / 4.0f) - 0.5);

						e.lock()->teleport({ sx, sy });
						e.lock()->layer = layers_counter;

						lua_rawgeti(L, LUA_REGISTRYINDEX, creator_function_ref);	//Restore the creator from registry

						lua_createtable(L, 0, 0);

						push_number_to_table(L, "x", sx);
						push_number_to_table(L, "y", sy);
						push_number_to_table(L, "layer", static_cast<float>(layers_counter));
						push_number_to_table(L, "object_layer", static_cast<float>(object_layers_counter));

						lua_pushstring(L, "entity");
						push_entity(L, e);
						lua_settable(L, -3);

						bool templated = object.contains("template") && object.at("template").is_string();
						nlohmann::json obj_template;

						if (templated)
							obj_template = load_template(tilemap_folder + std::string(object.at("template")));

						nlohmann::json* data;
						if (templated)
							data = &obj_template;
						else
							data = &object;

						push_string_to_table(L, "name", std::string(data->at("name")).c_str());
						push_string_to_table(L, "class", std::string(data->at("type")).c_str());

						if (data->contains("width"))
							push_number_to_table(L, "width", 
								static_cast<float>(float(data->at("width")) / common::pixels_per_world_unit)
							);

						if (data->contains("height"))
							push_number_to_table(L, "height",
								static_cast<float>(float(data->at("height")) / common::pixels_per_world_unit)
							);

						auto load_properites = [&](nlohmann::json source)
						{
							for (auto& prop : source.at("properties"))
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
									error_handling::crash(error_handling::error_source::core, 
										"[_en_create_entities_from_tilemap]",
										"Unsuported property type: " + type);

								lua_settable(L, -3);
							}
						};

						if (obj_template.contains("properties") && obj_template.at("properties").is_array())
							load_properites(obj_template);

						if (object.contains("properties") && object.at("properties").is_array())
							load_properites(object);

						auto err = lua_pcall(L, 1, 0, 0);
						if (err != LUA_OK)
							error_handling::crash(error_handling::error_source::core, 
								"[_en_create_entities_from_tilemap]", 
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

			int _en_load_custom_data(lua_State* L)
			{
				std::string f_name = "[_en_load_custom_data]";
				auto path = load_asset_path(L, 1, f_name);
				auto data_asset = assets::cast_asset<assets::custom_data>(common::assets_manager->safe_get_asset(path)).lock();
				auto data = data_asset->access_data();

				dump_json_to_table(L, data.get(), f_name);

				return 1;
			}

			int _en_time_period_to_physics(lua_State* L)
			{
				float dt = static_cast<float>(lua_tonumber(L, 1));
				lua_pushnumber(L, dt * common::physics_time_dilation_mod);
				return 1;
			}

			int _en_set_physics_time_dilation(lua_State* L)
			{
				float new_dilation = static_cast<float>(lua_tonumber(L, 1));
				common::physics_time_dilation_mod = new_dilation;
				return 0;
			}

			int _en_save_data(lua_State* L)
			{
				std::string filename = lua_tostring(L, 1);
				filesystem::set_saved_directory_enabled(true);

				nlohmann::json data;
				dump_table_to_json(L, &data, 2, "[_en_save_data]");

				auto file = filesystem::create_file(
					"saved/" + common::mods_manager->get_current_mod_folder_name() + '/' + filename + ".json"
				);			
				file << data;
				file.close();

				filesystem::set_saved_directory_enabled(false);
				return 0;
			}

			int _en_load_data(lua_State* L)
			{
				std::string filename = lua_tostring(L, 1);
				filesystem::set_saved_directory_enabled(true);

				auto file = filesystem::load_file(
					"saved/" + common::mods_manager->get_current_mod_folder_name()  + '/' + filename + ".json"
				);

				auto data = nlohmann::json::parse(file);
				file.close();

				dump_json_to_table(L, &data, "[_en_load_data]");

				filesystem::set_saved_directory_enabled(false);

				return 1;
			}

			void register_shared(lua_State* L)
			{
				lua_register(L, "_en_load_scene", _en_load_scene);
				lua_register(L, "_en_unload_scene", _en_unload_scene);
				lua_register(L, "_en_get_entities_in_scene", _en_get_entities_in_scene);
				lua_register(L, "_en_create_entities_from_tilemap", _en_create_entities_from_tilemap);
				lua_register(L, "_en_viewport_to_world", _en_viewport_to_world);
				lua_register(L, "_en_load_custom_data", _en_load_custom_data);
				lua_register(L, "_en_time_period_to_physics", _en_time_period_to_physics);
				lua_register(L, "_en_set_physics_time_dilation", _en_set_physics_time_dilation);
				lua_register(L, "_en_save_data", _en_save_data);
				lua_register(L, "_en_load_data", _en_load_data);
			}
		}
	}
}