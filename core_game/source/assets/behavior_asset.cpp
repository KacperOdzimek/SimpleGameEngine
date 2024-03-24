#include "behavior_asset.h"
#include "source/common/common.h"
#include "source/behaviors/behaviors_manager.h"

assets::behavior::behavior(std::string& lua_file_path)
{
    name = common::behaviors_manager->create_functions_table(lua_file_path);
}

assets::behavior::~behavior()
{
    common::behaviors_manager->destroy_functions_table(name);
}