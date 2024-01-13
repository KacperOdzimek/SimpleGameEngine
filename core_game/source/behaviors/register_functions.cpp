#include "register_functions.h"

extern "C"
{
#include "include/lua_5_4_2/include/lua.h"
#include "include/lua_5_4_2/include/lauxlib.h"
#include "include/lua_5_4_2/include/lualib.h"
}

#include "source/behaviors_shared/entities_functions.h"
#include "source/behaviors_shared/components_functions.h"
#include "source/behaviors_shared/assets_functions.h"

void behaviors::register_functions(lua_State* L)
{
	functions::entities::register_functions(L);

	functions::assets::register_functions(L);
}