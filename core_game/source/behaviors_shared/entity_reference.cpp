#include "entity_reference.h"

#include "utilities.h"

using namespace behaviors;
using namespace lua_shared;

int entity_reference_destructor(lua_State* L)
{
	std::weak_ptr<::entities::entity>* e
		= reinterpret_cast<std::weak_ptr<::entities::entity>*>(luaL_checkudata(L, -1, "entity"));
	e->reset();
	return 0;
}

static const luaL_Reg entity_ref_metamehods[] = {
	  {"__gc", entity_reference_destructor},
	  {NULL, NULL}
};

void entity_reference::register_shared(lua_State* L)
{
    luaL_newmetatable(L, "entity");
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);
    lua_settable(L, -3);

    luaL_setfuncs(L, entity_ref_metamehods, 0);

    lua_remove(L, -1);
}