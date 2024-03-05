#include "load_asset.h"

#include "source/common/crash.h"
#include "source/filesystem/filesystem.h"
#include "source/utilities/hash_string.h"

#include "shader_asset.h"
#include "texture_asset.h"
#include "sprite_sheet.h"
#include "flipbook_asset.h"
#include "tileset_asset.h"
#include "tilemap_asset.h"
#include "behavior_asset.h"
#include "scene_asset.h"
#include "mesh_asset.h"
#include "input_config_asset.h"
#include "collision_config_asset.h"

#include <fstream>

namespace assets
{
	namespace loading
	{
		std::shared_ptr<asset> load_texture(const load_data& ld)
		{
			auto& header = *ld.header_data;

			std::shared_ptr<asset> texture_asset;

			if (!(header.contains("path") && header.at("path").is_string()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_texture]",
					"Invalid/Missing image path");

			std::string source_path = ld.package + std::string(header.at("path"));
			auto image = filesystem::load_image(source_path);

			texture_asset = std::make_shared<assets::texture>(image.get());

			return texture_asset;
		}

		std::shared_ptr<asset> load_sprite_sheet(const load_data& ld)
		{
			auto& header = *ld.header_data;

			std::shared_ptr<asset> sprite_sheet_asset;

			if (!(header.contains("path") && header.at("path").is_string()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_sprite_sheet]",
					"Invalid/Missing image path");

			std::string source_path = ld.package + std::string(header.at("path"));
			auto image = filesystem::load_image(source_path);

			if (!(header.contains("sprite_width") && header.at("sprite_width").is_number_integer()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_sprite_sheet]",
					"Invalid/Missing sprite_width");

			unsigned int sprite_width = header.at("sprite_width");

			if (!(header.contains("sprite_height") && header.at("sprite_height").is_number_integer()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_sprite_sheet]",
					"Invalid/Missing sprite_height");

			unsigned int sprite_height = header.at("sprite_height");

			sprite_sheet_asset = std::make_shared<assets::sprite_sheet>(image.get(), sprite_width, sprite_height);

			return sprite_sheet_asset;
		}

		std::shared_ptr<asset> load_flipbook(const load_data& ld)
		{
			auto& header = *ld.header_data;

			std::shared_ptr<asset> flipbook_asset;

			if (!(header.contains("path") && header.at("path").is_string()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_flipbook]",
					"Invalid/Missing image path");

			std::string source_path = ld.package + std::string(header.at("path"));
			auto image = filesystem::load_image(source_path);

			if (!(header.contains("sprite_width") && header.at("sprite_width").is_number_integer()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_flipbook]",
					"Invalid/Missing sprite_width");

			unsigned int sprite_width = header.at("sprite_width");

			if (!(header.contains("sprite_height") && header.at("sprite_height").is_number_integer()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_flipbook]",
					"Invalid/Missing sprite_height");

			unsigned int sprite_height = header.at("sprite_height");

			std::map<uint32_t, flipbook::animation> animations;

			if (!(header.contains("animations") && header.at("animations").is_object()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_flipbook]",
					"Invalid/Missing animations");

			for (auto animation = header.at("animations").begin(); animation != header.at("animations").end(); ++animation)
			{
				std::string name = animation.key();
				if (!(animation.value().is_object() && 
					animation.value().contains("fps") && animation.value().at("fps").is_number() &&
					animation.value().contains("frames") && animation.value().at("frames").is_array()
				))
					error_handling::crash(error_handling::error_source::core, "[loading::load_flipbook]",
						"Each animation should be an object containg fps (number) and frames (array of ints)");

				flipbook::animation anim;
				anim.frames_per_second = animation.value().at("fps");

				for (auto& frame : animation.value().at("frames"))
					if (!frame.is_number_integer())
						error_handling::crash(error_handling::error_source::core, "[loading::load_flipbook]",
							"Each animation frame should be an int");
					else
						anim.frames.push_back(frame);

				animations.insert({utilities::hash_string(name), std::move(anim)});
			}

			flipbook_asset = std::make_shared<assets::flipbook>(image.get(), sprite_width, sprite_height, animations);

			return flipbook_asset;
		}

		std::shared_ptr<asset> load_tileset(const load_data& ld)
		{
			auto& header = *ld.header_data;

			std::shared_ptr<asset> tileset_asset;

			if (!(header.contains("path") && header.at("path").is_string()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_tileset]",
					"Invalid/Missing image path");
			std::string source_path = ld.package + std::string(header.at("path"));
			auto image = filesystem::load_image(source_path);

			if (!(header.contains("tile_width") && header.at("tile_width").is_number_integer()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_tileset]",
					"Invalid/Missing tile_width");
			unsigned int tile_width = header.at("tile_width");

			if (!(header.contains("tile_height") && header.at("tile_height").is_number_integer()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_tileset]",
					"Invalid/Missing tile_height");
			unsigned int tile_height = header.at("tile_height");

			if (!(header.contains("collisions") && header.at("collisions").is_array()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_tileset]",
					"Invalid/Missing collisions");
			std::vector<int> layers;
			for (auto tile_id : header.at("collisions"))
			{
				if (!tile_id.is_number_integer())
					error_handling::crash(error_handling::error_source::core, "[loading::load_tileset]",
						"Each tile_id in collisions should be an integer");
				layers.push_back(tile_id);
			}

			tileset_asset = std::make_shared<assets::tileset>(image.get(), tile_width, tile_height, layers);

			return tileset_asset;
		}

		std::shared_ptr<asset> load_tilemap(const load_data& ld)
		{
			auto& header = *ld.header_data;

			if (!(header.contains("path") && header.at("path").is_string()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_tilemap]",
					"Invalid/Missing path");

			std::string file_path = ld.package + std::string(header.at("path"));
			auto file = filesystem::load_file(file_path);
			if (file.fail())
				error_handling::crash(error_handling::error_source::core, "[asset_manager::load_tilemap]", 
					"Missing .tmj tilemap: " + header.at("path"));
			nlohmann::json source = nlohmann::json::parse(file);
			file.close();

			if (!(source.contains("width") && source.at("width").is_number_integer()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_tilemap]",
					"Source file is missing width");
			unsigned int width = source.at("width");

			if (!(source.contains("height") && source.at("height").is_number_integer()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_tilemap]",
					"Source file is missing height");
			unsigned int height = source.at("height");

			std::vector<tilemap::layer> layers;
			if (!(source.contains("layers") && source.at("layers").is_array()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_tilemap]",
					"Source file is missing layers");

			int layers_iterator = 0;
			for (auto& layer : source.at("layers"))
			{
				layers.push_back({});

				if (!layer.is_object()) 
					error_handling::crash(error_handling::error_source::core, "[loading::load_tilemap]","Invalid layer");

				if (!(layer.contains("data") && layer.at("data").is_array()))
					error_handling::crash(error_handling::error_source::core, "[loading::load_tilemap]", "Invalid layer");

				tilemap::row current_row;
				for (auto& tile : layer.at("data"))
				{
					if (current_row.size() == width)
					{
						layers.at(layers_iterator).push_back(std::move(current_row));
						current_row = {};
					}
					current_row.push_back(tile);
				}
				layers.at(layers_iterator).push_back(std::move(current_row));
				layers_iterator++;
			}

			std::shared_ptr<asset> tilemap_asset 
				= std::make_shared<assets::tilemap>(width, height, layers);
			return tilemap_asset;
		}

		std::shared_ptr<asset> load_shader(const load_data& ld)
		{
			auto& header = *ld.header_data;

			std::vector<std::string> layout = { "" };
			std::string vertex_code;
			std::string fragment_code;

			if (!(header.contains("path") && header.at("path").is_string()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_shader]",
					"Invalid/Missing shader path");

			std::string source_path = ld.package + std::string(header.at("path"));
			auto source_file = filesystem::load_file(source_path);

			source_file.seekg(0, std::ios::end);
			size_t size = source_file.tellg();
			std::string source(size, ' ');
			source_file.seekg(0);
			source_file.read(&source[0], size);
			source_file.close();

			//seek start
			int i = 0;
			auto seek_start = [&]()
			{
				for (; i < source.size(); i++)
					if (source[i] == '<')
						break;
				i++;
			};
			seek_start();

			//load layout
			for (; i < source.size(); i++)
			{
				if (source[i] == '>')
					break;
				else if (source[i] == ',')
					layout.push_back("");
				else if (source[i] != ' ' && source[i] != '\r' && source[i] != '\t' && source[i] != '\n')
					layout.back().push_back(source[i]);
			}				

			//load vertex
			seek_start();
			for (; i < source.size(); i++)
			{
				if (source[i] == '>')
					break;
				else
					vertex_code.push_back(source[i]);
			}

			//load fragment
			seek_start();
			for (; i < source.size(); i++)
			{
				if (source[i] == '>')
					break;
				else
					fragment_code.push_back(source[i]);
			}
		
			std::vector<uint32_t> hashed_layout;
			for (auto& element : layout)
				hashed_layout.push_back(utilities::hash_string(element));

			return std::make_shared<shader>(vertex_code, fragment_code, hashed_layout);
		}

		std::shared_ptr<asset> load_behavior(const load_data& ld)
		{
			auto& header = *ld.header_data;

			std::shared_ptr<asset> behavior_asset;

			if (!(header.contains("path") && header.at("path").is_string()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_behavior]",
					"Invalid/Missing behavior path");

			std::string source_path = ld.package + std::string(header.at("path"));
			behavior_asset = std::make_shared<assets::behavior>(source_path);
			
			return behavior_asset;
		}

		std::shared_ptr<asset> load_scene(const load_data& ld)
		{
			auto& header = *ld.header_data;

			std::shared_ptr<asset> scene_asset;

			if (!(header.contains("path") && header.at("path").is_string()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_scene]",
					"Invalid/Missing scene script path");

			std::string source_path = ld.package + std::string(header.at("path"));
			scene_asset = std::make_shared<assets::scene>(source_path);

			return scene_asset;
		}

		std::shared_ptr<asset> load_mesh(const load_data& ld)
		{
			auto& header = *ld.header_data;

			std::shared_ptr<asset> mesh_asset;

			if (!(header.contains("path") && header.at("path").is_string()))
				error_handling::crash(error_handling::error_source::core, "[loading::load_mesh]",
					"Invalid/Missing mesh path");

			std::string source_path = ld.package + std::string(header.at("path"));
			std::fstream source_file = filesystem::load_file(source_path);

			source_file.seekg(0, std::ios::end);
			size_t size = source_file.tellg();
			std::string source(size, ' ');
			source_file.seekg(0);
			source_file.read(&source[0], size);
			source_file.close();

			int i = 0;

			auto seek_start = [&]()
			{
				for (; i < source.size(); i++)
					if (source[i] == '<')
						break;
				i++;
			};

			seek_start();

			std::string current_value;
			std::vector<float> vertices;

			for (; i < source.size(); i++)
			{
				if (source[i] == ',' || source[i] == '>')
				{
					if (!current_value.empty())
						vertices.push_back(::atof(current_value.c_str()));
					current_value.clear();
				}
				else if (source[i] != ' ' && source[i] != '\r' && source[i] != '\t' && source[i] != '\n')
					current_value.push_back(source[i]);

				if (source[i] == '>')
					break;
			}

			seek_start();

			std::vector<int> indicies;
			for (; i < source.size(); i++)
			{
				if (source[i] == ',' || source[i] == '>')
				{
					if (!current_value.empty())
						indicies.push_back(::atoi(current_value.c_str()));
					current_value.clear();
				}
				else if (source[i] != ' ' && source[i] != '\r' && source[i] != '\t' && source[i] != '\n')
					current_value.push_back(source[i]);

				if (source[i] == '>')
					break;
			}

			if (indicies.size() == 0)
				return std::make_shared<assets::mesh>(vertices);
			return std::make_shared<assets::mesh>(vertices, indicies);
		}

		std::shared_ptr<asset> load_input_config(const load_data& ld)
		{
			auto& header = *ld.header_data;
			std::shared_ptr<asset> input_config_asset;

			//actions mappings
			if (!header.contains("action_mappings"))
				error_handling::crash(error_handling::error_source::core, "[loading::load_input_config]",
					"Input config is missing action_mappings");

			auto actions = header.at("action_mappings");
			std::unordered_map<std::string, input::action_mapping> actions_map;

			for (auto action = actions.begin(); action != actions.end(); ++action)
			{
				input::action_mapping am;

				if (!action.value().is_array())
					error_handling::crash(error_handling::error_source::core, "[loading::load_input_config]",
						"Action mapping should be an array of keys");

				for (auto& key : action.value())
				{
					if (!key.is_string())
						error_handling::crash(error_handling::error_source::core, "[loading::load_input_config]",
							"Action mapping keys should be strings");
					std::string button_name;
					key.get_to(button_name);
					am.keys.push_back(input::get_key_from_key_name(button_name));
				}

				actions_map.insert({action.key(), am});
			}

			//axis mappings
			if (!header.contains("axis_mappings"))
				error_handling::crash(error_handling::error_source::core, "[loading::load_input_config]",
					"Input config is missing axis_mappings");

			auto axises = header.at("axis_mappings");
			std::unordered_map<std::string, input::axis_mapping> axis_map;

			for (auto axis = axises.begin(); axis != axises.end(); ++axis)
			{
				input::axis_mapping am;

				if (!axis.value().is_object())
					error_handling::crash(error_handling::error_source::core, "[loading::load_input_config]",
						"Axis mapping should be an object");

				for (auto key = axis.value().begin(); key != axis.value().end(); ++key)
				{
					if (!key.value().is_number())
						error_handling::crash(error_handling::error_source::core, "[loading::load_input_config]",
							"Axis key should be an number");

					input::key k = input::get_key_from_key_name(key.key());
					k.axis_value = key.value();
					am.keys.push_back(k);
				}

				axis_map.insert({axis.key(), am});
			}

			input_config_asset = std::make_shared<assets::input_config>(std::move(actions_map), std::move(axis_map));
			return input_config_asset;
		}

		std::shared_ptr<asset> load_collision_config(const load_data& ld)
		{
			auto& header = *ld.header_data;

			std::shared_ptr<asset> collision_config_asset;

			//load body_types
			std::map<uint32_t, uint8_t> body_types_loaded;
			{
				auto& body_types = header.at("body_types");
				if (!body_types.is_object())
					error_handling::crash(error_handling::error_source::core, "[loading::load_collision_config]",
						"Invalid collision config: \nbody_types should be an object");

				for (auto body_type = body_types.begin(); body_type != body_types.end(); ++body_type)
				{
					if (!body_type.value().is_number())
						error_handling::crash(error_handling::error_source::core, "[loading::load_collision_config]",
							"Invalid collision config: \nbody_types element value should be an number");
					uint8_t value;
					body_type.value().get_to(value);
					body_types_loaded.insert({ utilities::hash_string(body_type.key()), value });
				}
			}
			
			//load all collsion_presets
			std::map<uint32_t, physics::collision_preset> collision_presets_loaded;
			std::map<uint32_t, std::string> collision_presets_names;
			{
				auto& header = *ld.header_data;

				auto& collision_presets = header.at("collision_presets");
				if (!collision_presets.is_object())
					error_handling::crash(error_handling::error_source::core, "[loading::load_collision_config]",
						"Invalid collision config: \ncollision_presets should be an object");
				//load collsion_presets
				for (auto collision_preset = collision_presets.begin(); collision_preset != collision_presets.end(); ++collision_preset)
				{
					if (!collision_preset.value().is_object())
						error_handling::crash(error_handling::error_source::core, "[loading::load_collision_config]",
					"Invalid collision config: \ncollsion presets should be objects made of \n[string] body_type\n[object] responses");

					//load collsion_preset::body_type
					uint8_t body_type_id;
					{
						std::string body_type_name;
						collision_preset.value().at("body_type").get_to(body_type_name);
						auto bt_id_itr = body_types_loaded.find(utilities::hash_string(body_type_name));
						if (bt_id_itr == body_types_loaded.end())
							error_handling::crash(error_handling::error_source::core, "[loading::load_collision_config]",
								"Invalid collision config: \ninvalid body_type in collsion preset");
						body_type_id = bt_id_itr->second;
					}
					//load collsion_preset::responses
					std::array<physics::collision_response, 14> responses_loaded = { physics::collision_response::ignore };
					{
						auto& responses = collision_preset.value().at("responses");
						if (!responses.is_object())
							error_handling::crash(error_handling::error_source::core, "[loading::load_collision_config]",
								"Invalid collision config: \nresposnses in collision_presets should be an object");
						for (auto response = responses.begin(); response != responses.end(); ++response)
						{
							std::string target_body_type = response.key();
							if (!response.value().is_string())
								error_handling::crash(error_handling::error_source::core, "[loading::load_collision_config]",
									"Invalid collision config : \nresposnse in collision_presets should either \n\"ignore\" \n\"overlap\" \n\"collide\"");
							std::string response_response_type = response.value();
							auto bt_id_itr = body_types_loaded.find(utilities::hash_string(response.key()));
							if (bt_id_itr == body_types_loaded.end())
								error_handling::crash(error_handling::error_source::core, "[loading::load_collision_config]", 
									"Invalid collision config: \ninvalid body type name in collsion presets response");
							uint8_t target_body_type_id = bt_id_itr->second;
							
							if (response_response_type == "collide")
								responses_loaded.at(target_body_type_id) = physics::collision_response::collide;
							else if (response_response_type == "overlap")
								responses_loaded.at(target_body_type_id) = physics::collision_response::overlap;
							else if (response_response_type == "ignore")
								responses_loaded.at(target_body_type_id) = physics::collision_response::ignore;
							else
								error_handling::crash(error_handling::error_source::core, "[loading::load_collision_config]", 
									"Invalid collision config: \ninvalid response type");
						}
						collision_presets_loaded.insert({
							utilities::hash_string(collision_preset.key()), physics::gen_flag(body_type_id, responses_loaded)
						});
						collision_presets_names.insert({ utilities::hash_string(collision_preset.key()), collision_preset.key()});
					}
				}
			}

			collision_config_asset = std::make_shared<assets::collision_config>(body_types_loaded, collision_presets_loaded, collision_presets_names);
			return collision_config_asset;
		}
	}
}