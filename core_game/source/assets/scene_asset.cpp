#include "scene_asset.h"
#include "source/common/common.h"
#include "source/behaviors/behaviors_manager.h"

assets::scene::scene(std::string& lua_file_path)
{
    name = common::behaviors_manager->create_functions_table(lua_file_path);
}