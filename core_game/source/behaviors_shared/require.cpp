#include "require.h"

#include "utilities.h"

#include "source/filesystem/filesystem.h"

using namespace behaviors;
using namespace lua_functions;

int require_func(lua_State* L)
{
	std::string relative_path = lua_tostring(L, 1);
	std::string global_path = filesystem::get_global_path(relative_path) + ".lua";

    //TODO: manage modules lifetime
    luaL_dofile(L, global_path.c_str());

	return 1;
}

void require::register_functions(lua_State* L)
{
    //change native require function name so we can provide our own
    lua_getglobal(L, "require");
    lua_setglobal(L, "raw_require");

	lua_register(L, "require", require_func);
}