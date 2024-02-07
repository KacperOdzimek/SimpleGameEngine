#include "assets_manager.h"

#include "source/filesystem/filesystem.h"

#include "source/common/abort.h"
#include "include/nlohmann/json.hpp"

#include "source/utilities/hash_string.h"
#include "source/assets/load_asset.h"

using namespace assets;

struct assets_manager::implementation
{
    //pointers to existing / expired assets
    std::map<uint32_t, std::weak_ptr<asset>> assets;
    //pointers to just created assets
    //this is cleared every frame, so if loaded asset don't 
    //have reference outside of manager at the end of the frame it will be unloaded
    std::vector<std::shared_ptr<asset>> new_assets;
    //assets that are kept alive by asset_manager
    //to add / remove to this vector use lock_asset / unlock_asset
    std::map<uint32_t, std::shared_ptr<asset>> locked_assets;
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
    return impl->assets.at(hashed_name);
}

void assets_manager::load_asset(std::string path)
{
    auto file = filesystem::load_file(path + ".json");
    if (file.fail())
        abort("Missing asset: " + path);

    nlohmann::json data = nlohmann::json::parse(file);
    file.close();

    if (!data.contains("asset_type"))
        abort("Invalid asset: " + path + "\nMissing asset_type");

    auto at = data.at("asset_type");

    if (!at.is_string())
        abort("Invalid asset: " + path + "\nasset_type should be string");

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
    case utilities::hash_string("shader"):
        new_asset = loading::load_shader(load_data);
        break;
    case utilities::hash_string("behavior"):
        new_asset = loading::load_behavior(load_data);
        break;
    case utilities::hash_string("mesh"):
        new_asset = loading::load_mesh(load_data);
        break;
    case utilities::hash_string("collision_config") :
        new_asset = loading::load_collision_config(load_data);
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

    impl->new_assets.push_back(
        new_asset
    );
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