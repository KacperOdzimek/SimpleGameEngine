#include "assets_manager.h"

#include "source/filesystem/filesystem.h"

#include "source/common/crash.h"
#include "include/nlohmann/json.hpp"

#include "source/utilities/hash_string.h"
#include "source/assets/load_asset.h"

#include <unordered_map>

using namespace assets;

struct assets_manager::implementation
{
    //pointers to existing / expired assets
    std::unordered_map<uint32_t, std::weak_ptr<asset>> assets;
    //pointers to just created assets
    //this is cleared every frame, so if loaded asset don't 
    //have reference outside of manager at the end of the frame it will be unloaded
    std::vector<std::shared_ptr<asset>> new_assets;
    //assets that are kept alive by asset_manager
    //to add / remove to this vector use lock_asset / unlock_asset
    std::unordered_map<uint32_t, std::shared_ptr<asset>> locked_assets;
};

assets_manager::assets_manager()
{
    impl = new implementation;
}

assets_manager::~assets_manager()
{
    delete impl;
}

std::weak_ptr<asset> assets_manager::get_asset(uint32_t hashed_name)
{
    auto itr = impl->assets.find({ hashed_name });
    if (itr == impl->assets.end())
        error_handling::crash(error_handling::error_source::core,
            "[asset_manager::get_asset]", "Missing asset: " + std::to_string(hashed_name));
    return  itr->second;
}

std::weak_ptr<asset> assets_manager::safe_get_asset(std::string path)
{
    auto itr = impl->assets.find({ utilities::hash_string(path) });
    if (itr != impl->assets.end() && !itr->second.expired())
        return itr->second;
    load_asset(path);
    return get_asset(utilities::hash_string(path));
}

void assets_manager::load_asset(std::string path)
{
    auto file = filesystem::load_file(path + ".json");
    if (file.fail())
        error_handling::crash(error_handling::error_source::core, "[asset_manager::load_asset]", "Missing asset: " + path); 

    nlohmann::json data = nlohmann::json::parse(file);
    file.close();

    if (!data.contains("asset_type"))
        error_handling::crash(error_handling::error_source::core, 
            "[asset_manager::load_asset]", "Invalid asset: " + path + " Missing asset_type");

    auto at = data.at("asset_type");

    if (!at.is_string())
        error_handling::crash(error_handling::error_source::core,
            "[asset_manager::load_asset]", "Invalid asset: " + path + " asset_type should be string");

    std::string asset_type;
    at.get_to(asset_type);
    uint32_t hashed_asset_type = utilities::hash_string(asset_type);

    loading::load_data load_data;
    load_data.header_data = &data;
    load_data.package = filesystem::get_package(path);

    std::shared_ptr<asset> new_asset = nullptr;
    switch (hashed_asset_type)
    {
    case utilities::hash_string("texture"):
        new_asset = loading::load_texture(load_data);
        break;
    case utilities::hash_string("sprite_sheet"):
        new_asset = loading::load_sprite_sheet(load_data);
        break;
    case utilities::hash_string("tileset"):
        new_asset = loading::load_tileset(load_data);
        break;
    case utilities::hash_string("tilemap"):
        new_asset = loading::load_tilemap(load_data);
        break;
    case utilities::hash_string("shader"):
        new_asset = loading::load_shader(load_data);
        break;
    case utilities::hash_string("behavior"):
        new_asset = loading::load_behavior(load_data);
        break;
    case utilities::hash_string("mesh"):
        new_asset = loading::load_mesh(load_data);
        break;
    case utilities::hash_string("input_config"):
        new_asset = loading::load_input_config(load_data);
        break;
    case utilities::hash_string("collision_config") :
        new_asset = loading::load_collision_config(load_data);
        break;
    default:
        error_handling::crash(error_handling::error_source::core,
            "[asset_manager::load_asset]", "Invalid asset: " + path + " invalid asset_type");
    }

    if (new_asset == nullptr)
        error_handling::crash(error_handling::error_source::core,
            "[asset_manager::load_asset]", "Failed to load asset: " + path);

    impl->assets.insert(
        { 
            utilities::hash_string(path),
            new_asset 
        }
    );

    impl->new_assets.push_back(
        new_asset
    );

    new_asset->package_name = path;
}

void assets_manager::unload_unreferenced_assets()
{
    impl->new_assets.clear();
}

void assets_manager::lock_asset(uint32_t hashed_name)
{
    impl->locked_assets.insert({ hashed_name, get_asset(hashed_name).lock() });
}

void assets_manager::unlock_asset(uint32_t hashed_name)
{
    auto itr = impl->locked_assets.find(hashed_name);
    if (itr != impl->locked_assets.end())
        impl->locked_assets.erase(itr);
}