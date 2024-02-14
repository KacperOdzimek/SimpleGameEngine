#include "register_functions.h"

extern "C"
{
#include "include/lua_5_4_2/include/lua.h"
#include "include/lua_5_4_2/include/lauxlib.h"
#include "include/lua_5_4_2/include/lualib.h"
}

#include "behaviors_manager.h"
#include "behaviors_database.h"

#include "source/assets/assets_manager.h"

#include "source/utilities/hash_string.h"

#include "source/entities/entity.h"
#include "source/entities/component.h"

#include "source/common/common.h"

inline std::shared_ptr<::entities::entity> load_entity(lua_State* L, int arg_id)
{
	return reinterpret_cast<std::weak_ptr<::entities::entity>*>(lua_tointeger(L, arg_id))->lock();
}

inline behaviors::database* load_database(lua_State* L, int arg_id)
{
	return reinterpret_cast<::behaviors::database*>(lua_tointeger(L, arg_id));
}

inline uint32_t load_id(lua_State* L, int arg_id)
{
	if (lua_isnumber(L, arg_id) || lua_isinteger(L, arg_id))
		return lua_tointeger(L, arg_id);
	return ::utilities::hash_string(lua_tostring(L, arg_id));
}

template<class comp_class>
inline comp_class* load_component(lua_State* L, int entity_ptr_pos = 1, int component_id_pos = 2)
{
	auto e = load_entity(L, entity_ptr_pos);
	uint32_t comp = load_id(L, component_id_pos);
	return dynamic_cast<comp_class*>(e->get_component(comp));
}

/*inline entities::geometry_draw_settings load_geometry_draw_settings(lua_State* L, int pos)
{
	entities::geometry_draw_settings gds;
	lua_pushnil(L);
	if (lua_istable(L, pos))
	{
		while (lua_next(L, pos) != 0)
		{
			if (lua_isstring(L, -2))
			{
				std::string key = lua_tostring(L, -2);
				if (key == "shader")
				{
					gds.shader = assets::cast_asset<assets::shader>(common::assets_manager->get_asset(load_id(L, -1))).lock();
				}
				else if (key == "textures")
				{
					if (lua_istable(L, pos + 2))
					{
						lua_pushnil(L);
						while (lua_next(L, pos + 2) != 0)
						{
							uint32_t txt_id = load_id(L, -1);
							gds.textures.push_back(
								assets::cast_asset<assets::texture>(common::assets_manager->get_asset(load_id(L, -1))).lock()
							);
							lua_pop(L, 1);
						}
					}
				}
			}
			
			lua_pop(L, 1);
		}
	}

	return gds;
}*/

#include "source/behaviors_shared/databases_functions.h"
#include "source/behaviors_shared/entities_functions.h"
#include "source/behaviors_shared/add_component_functions.h"
#include "source/behaviors_shared/components_functions.h"
#include "source/behaviors_shared/input_functions.h"

void behaviors::register_functions(lua_State* L)
{
	lua_functions::databases::register_functions(L);
	lua_functions::entities::register_functions(L);
	lua_functions::add_component::register_functions(L);
	lua_functions::components::register_functions(L);
	lua_functions::input::register_functions(L);
}