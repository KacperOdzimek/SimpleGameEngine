#include "register_functions.h"

extern "C"
{
#include "include/lua_5_4_2/include/lua.h"
#include "include/lua_5_4_2/include/lauxlib.h"
#include "include/lua_5_4_2/include/lualib.h"
}

#include "source/utilities/hash_string.h"
#include "source/entities/entity.h"
#include "source/entities/component.h"

inline entities::entity* load_entity(lua_State* L, int arg_id)
{
	return reinterpret_cast<::entities::entity*>(lua_tointeger(L, arg_id));
}

inline uint32_t load_id(lua_State* L, int arg_id)
{
	if (lua_isstring(L, arg_id))
		return ::utilities::hash_string(lua_tostring(L, arg_id));
	return lua_tointeger(L, arg_id);
}

template<class comp_class>
inline comp_class* load_component(lua_State* L, int entity_ptr_pos = 1, int component_id_pos = 2)
{
	auto e = load_entity(L, entity_ptr_pos);
	uint32_t comp = load_id(L, component_id_pos);
	return dynamic_cast<comp_class*>(e->get_component(comp));
}

#include "source/behaviors_shared/entities_functions.h"
#include "source/behaviors_shared/add_component_functions.h"
#include "source/behaviors_shared/components_functions.h"

void behaviors::register_functions(lua_State* L)
{
	lua_functions::entities::register_functions(L);
	lua_functions::add_component::register_functions(L);
	lua_functions::components::register_functions(L);
}