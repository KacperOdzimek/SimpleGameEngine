#include "register_functions.h"

extern "C"
{
#include "include/lua_5_4_2/include/lua.h"
#include "include/lua_5_4_2/include/lauxlib.h"
#include "include/lua_5_4_2/include/lualib.h"
}

#include "source/entities/entity.h"

int _set_entity_location(lua_State* L)
{
	auto pointer = reinterpret_cast<entities::entity*>(lua_tointeger(L, 1));
	float x = (float)lua_tonumber(L, 2);
	float y = (float)lua_tonumber(L, 3);
	pointer->position = { x, y };
	return 0;
}

int _get_entity_location(lua_State* L)
{
	auto pointer = reinterpret_cast<entities::entity*>(lua_tointeger(L, 1));
	lua_pushnumber(L, pointer->position.x);
	lua_pushnumber(L, pointer->position.y);
	return 2;
}

void behaviors::register_functions(lua_State* L)
{
	lua_register(L, "_set_entity_loc", _set_entity_location);
	lua_register(L, "_get_entity_loc", _get_entity_location);
}