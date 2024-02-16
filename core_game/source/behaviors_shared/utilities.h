#pragma once
#include "source/common/common.h"
#include "source/common/abort.h"

#include "source/assets/asset.h"
#include "source/assets/assets_manager.h"

#include "source/assets/mesh_asset.h"
#include "source/assets/shader_asset.h"
#include "source/assets/texture_asset.h"

extern "C"
{
#include "include/lua_5_4_2/include/lua.h"
#include "include/lua_5_4_2/include/lauxlib.h"
#include "include/lua_5_4_2/include/lualib.h"
}

#include "source/entities/entity.h"
#include "source/entities/component.h"

#include "source/utilities/hash_string.h"

#include "source/rendering/render_config.h"

inline std::shared_ptr<::entities::entity> load_entity(lua_State* L, int arg_id)
{
	return reinterpret_cast<std::weak_ptr<::entities::entity>*>(lua_tointeger(L, arg_id))->lock();
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

inline rendering::render_config load_render_config(lua_State* L, int arg_id)
{
	rendering::render_config rc{};

	lua_pushnil(L);
	if (lua_istable(L, arg_id))
	{
		while (lua_next(L, arg_id) != 0)
		{
			if (lua_isstring(L, -2))
			{
				std::string key = lua_tostring(L, -2);
				if (key == "shader")
				{
					rc.material = assets::cast_asset<assets::shader>(common::assets_manager->get_asset(load_id(L, -1))).lock();
				}
				else if (key == "mesh")
				{
					rc.mesh = assets::cast_asset<assets::mesh>(common::assets_manager->get_asset(load_id(L, -1))).lock();
				}
				else if (key == "textures")
				{
					if (lua_istable(L, arg_id + 2))
					{
						lua_pushnil(L);
						while (lua_next(L, arg_id + 2) != 0)
						{
							uint32_t txt_id = load_id(L, -1);
							rc.textures.push_back(
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

	if (rc.material == nullptr)
		abort("Render config is missing shader");
	else if (rc.mesh == nullptr)
		abort("Render config is missing mesh");

	return rc;
}