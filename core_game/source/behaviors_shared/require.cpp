#include "require.h"

#include "utilities.h"

#include "source/common/common.h"
#include "source/behaviors/behaviors_manager.h"

using namespace behaviors;
using namespace lua_shared;

int require_func(lua_State* L)
{
	std::string relative_path = lua_tostring(L, 1);
    common::behaviors_manager->require_module(relative_path);
	return 1;
}

void require::register_shared(lua_State* L)
{
    //change native require function name so we can provide new implementation
    lua_getglobal(L, "require");
    lua_setglobal(L, "raw_require");

	lua_register(L, "require", require_func);
}