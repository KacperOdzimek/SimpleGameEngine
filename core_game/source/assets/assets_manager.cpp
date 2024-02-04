#include "assets_manager.h"

#include "source/filesystem/filesystem.h"

#include "source/common/abort.h"
#include "include/nlohmann/json.hpp"

#include "source/utilities/hash_string.h"
#include "source/assets/load_asset.h"

using namespace assets;
using json = nlohmann::json;

struct assets_manager::implementation
{
    std::string mod_global_path;
    std::map<uint32_t, std::shared_ptr<asset>> assets;
};

assets_manager::assets_manager()
{
    impl = new implementation;
}

assets_manager::~assets_manager()
{
    delete impl;
}

void assets_manager::set_mod_path(std::string assets_global_path)
{
    impl->mod_global_path = assets_global_path;
}

std::weak_ptr<asset> assets_manager::get_asset(uint32_t hashed_name)
{
    return impl->assets.at(hashed_name);
}

void assets_manager::load_asset(std::string path)
{
    auto file = filesystem::load_json_file_relative_path(path, impl->mod_global_path);
    if (file.fail())
        abort("Missing asset: " + path);
    else
    {
        json data = json::parse(file);
        file.close();
        if (!data.contains("asset_type"))
            abort("Invalid asset: " + path + "\nMissing asset_type");
        else
        {
            auto at = data.at("asset_type");
            if (!at.is_string())
                abort("Invalid asset: " + path + "\nasset_type should be string");
            else
            {
                std::string asset_type;
                at.get_to(asset_type);
                uint32_t hashed_asset_type = utilities::hash_string(asset_type);

                std::shared_ptr<asset> new_asset = nullptr;
                switch (hashed_asset_type)
                {
                case utilities::hash_string("texture"):
                    new_asset = loading::load_texture(impl->mod_global_path, data);
                    break;
                case utilities::hash_string("shader"):
                    new_asset = loading::load_shader(impl->mod_global_path, data);
                    break;
                case utilities::hash_string("behavior"):
                    new_asset = loading::load_behavior(impl->mod_global_path, data);
                    break;
                case utilities::hash_string("collision_config") :
                    new_asset = loading::load_collision_config(impl->mod_global_path, data);
                    break;
                default:
                    abort("Invalid asset: " + path + "\nInvalid asset_type");
                }

                if (new_asset == nullptr)
                    abort("Invalid asset: " + path + "\nInvalid params or path");

                impl->assets.insert(
                    { 
                        utilities::hash_string(path),
                        new_asset 
                    }
                );
            }
        }
    } 
}

const std::string& assets_manager::get_assets_path()
{
    return impl->mod_global_path;
}