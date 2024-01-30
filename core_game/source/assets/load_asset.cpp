#include "load_asset.h"
#include "source/common/abort.h"
#include "source/utilities/hash_string.h"

#include "include/stb/stb_image.h"

#include "shader_asset.h"
#include "texture_asset.h"
#include "behavior_asset.h"
#include "collision_config_asset.h"

#include <fstream>

namespace assets
{
	namespace loading
	{
		std::shared_ptr<asset> load_texture(std::string& assets_folder_path, nlohmann::json& data)
		{
			std::shared_ptr<asset> texture_asset;

			if (!(data.contains("path") && data.at("path").is_string()))
				abort("Invalid asset: " + assets_folder_path + "\nMissing/Invalid path parameter");
			else
			{
				std::string source_path = data.at("path");

				int texture_width, texture_height, color_channels;
				stbi_set_flip_vertically_on_load(true);

				unsigned char* image_source = stbi_load((assets_folder_path + source_path).c_str(), 
					&texture_width, &texture_height, &color_channels, 0);

				if (image_source == nullptr)
					abort("Invalid asset: " + assets_folder_path + "\nNo image at given path");
				else
				{
					assets::texture::load_data texture_data;
					texture_data.color_channels = color_channels;
					texture_data.width = texture_width;
					texture_data.height = texture_height;
					texture_data.image_source = image_source;
					texture_asset = std::make_shared<assets::texture>(texture_data);
					stbi_image_free(image_source);
				}
			}
			return texture_asset;
		}

		std::shared_ptr<asset> load_shader(std::string& assets_folder_path, nlohmann::json& data)
		{
			std::vector<std::string> layout = { "" };
			std::string vertex_code;
			std::string fragment_code;

			if (!(data.contains("path") && data.at("path").is_string()))
				abort("Invalid asset: " + assets_folder_path + "\nMissing/Invalid path parameter");
			else
			{
				std::string source_path = data.at("path");
				std::fstream source_file;
				source_file.open(assets_folder_path + source_path);
				if (source_file.fail())
					abort("Invalid asset: " + assets_folder_path + source_path + "\nNo shader source file at given path");
				else
				{
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
				}
			}
		
			std::vector<uint32_t> hashed_layout;
			for (auto& element : layout)
				hashed_layout.push_back(utilities::hash_string(element));

			return std::make_shared<shader>(vertex_code, fragment_code, hashed_layout);
		}

		std::shared_ptr<asset> load_behavior(std::string& assets_folder_path, nlohmann::json& data)
		{
			std::shared_ptr<asset> behavior_asset;

			if (!(data.contains("path") && data.at("path").is_string()))
				abort("Invalid asset: " + assets_folder_path + "\nMissing/Invalid path parameter");
			else
			{
				std::string source_path = data.at("path");
				behavior_asset = std::make_shared<assets::behavior>(source_path);
			}
			
			return behavior_asset;
		}

		std::shared_ptr<asset> load_collision_config(std::string& assets_folder_path, nlohmann::json& data)
		{
			std::shared_ptr<asset> collision_config_asset;

			//load body_types
			std::map<uint32_t, uint8_t> body_types_loaded;
			{
				auto& body_types = data.at("body_types");
				if (!body_types.is_object())
					abort("Invalid asset: " + assets_folder_path + "\nbody_types should be an object");
				for (auto body_type = body_types.begin(); body_type != body_types.end(); ++body_type)
				{
					if (!body_type.value().is_number())
						abort("Invalid asset: " + assets_folder_path + "\nbody_types element value should be an number");
					uint8_t value;
					body_type.value().get_to(value);
					body_types_loaded.insert({ utilities::hash_string(body_type.key()), value });
				}
			}
			
			//load all collsion_presets
			std::map<uint32_t, physics::collision_preset> collision_presets_loaded;
			{
				auto& collision_presets = data.at("collision_presets");
				if (!collision_presets.is_object())
					abort("Invalid asset: " + assets_folder_path + "\collision_presets should be an object");
				//load collsion_presets
				for (auto collision_preset = collision_presets.begin(); collision_preset != collision_presets.end(); ++collision_preset)
				{
					if (!collision_preset.value().is_object())
						abort("Invalid asset: " + assets_folder_path + "\ncollsion presets should be objects made of \n[string] body_type\n[object] responses");

					//load collsion_preset::body_type
					uint8_t body_type_id;
					{
						std::string body_type_name;
						collision_preset.value().at("body_type").get_to(body_type_name);
						auto bt_id_itr = body_types_loaded.find(utilities::hash_string(body_type_name));
						if (bt_id_itr == body_types_loaded.end())
							abort("Invalid asset: " + assets_folder_path + "\ninvalid body_type in collsion preset");
						body_type_id = bt_id_itr->second;
					}
					//load collsion_preset::responses
					std::array<physics::collision_response, 14> responses_loaded = { physics::collision_response::ignore };
					{
						auto& responses = collision_preset.value().at("responses");
						if (!responses.is_object())
							abort("Invalid asset: " + assets_folder_path + "\nresposnses in collision_presets should be an object");
						for (auto response = responses.begin(); response != responses.end(); ++response)
						{
							std::string target_body_type = response.key();
							if (!response.value().is_string())
								abort(
									"Invalid asset: " + assets_folder_path + "\nresposnse in collision_presets should either \n\"ignore\" \n\"overlap\" \n\"collide\""
								);
							std::string response_response_type = response.value();
							auto bt_id_itr = body_types_loaded.find(utilities::hash_string(response.key()));
							if (bt_id_itr == body_types_loaded.end())
								abort("Invalid asset: " + assets_folder_path + "\ninvalid body type name in collsion presets response");
							uint8_t target_body_type_id = bt_id_itr->second;
							
							if (response_response_type == "collide")
								responses_loaded.at(target_body_type_id) = physics::collision_response::collide;
							else if (response_response_type == "overlap")
								responses_loaded.at(target_body_type_id) = physics::collision_response::overlap;
							else if (response_response_type == "ignore")
								responses_loaded.at(target_body_type_id) = physics::collision_response::ignore;
							else
								abort("Invalid asset: " + assets_folder_path + "\ninvalid response type");
						}
						collision_presets_loaded.insert({
							utilities::hash_string(collision_preset.key()), physics::gen_flag(body_type_id, responses_loaded)
						});
					}
				}
			}

			collision_config_asset = std::make_shared<assets::collision_config>(body_types_loaded, collision_presets_loaded);
			return collision_config_asset;
		}
	}
}