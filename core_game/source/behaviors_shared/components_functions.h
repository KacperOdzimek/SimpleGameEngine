#include "source/entities/entity.h"
#include "source/utilities/hash_string.h"

#include "source/assets/assets_manager.h"
#include "source/rendering/renderer.h"

#include "source/components/behavior.h"

#include "source/assets/asset.h"
#include "source/assets/behavior_asset.h"
#include "source/assets/collision_config_asset.h"

namespace behaviors
{
	namespace lua_functions
	{
		namespace components
		{
			/*
				Geometry
			*/

			int _c_g_set_entity_offset(lua_State* L)
			{
				auto geo = load_component<::entities::components::sprite>(L);
				geo->entity_offset.x = lua_tonumber(L, 3);
				geo->entity_offset.y = lua_tonumber(L, 4);
				return 0;
			}

			int _c_g_get_entity_offset(lua_State* L)
			{
				auto geo = load_component<::entities::components::sprite>(L);
				lua_pushnumber(L, geo->entity_offset.x);
				lua_pushnumber(L, geo->entity_offset.y);
				return 2;
			}

			/*
				Camera
			*/

			int _c_c_set_ortho_width(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L);
				float width = lua_tonumber(L, 3);
				if (camera != nullptr)
					camera->ortho_width = width;
				return 0;
			}

			int _c_c_get_ortho_width(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L);
				if (camera != nullptr)
					lua_pushnumber(L, camera->ortho_width);
				else
					lua_pushnumber(L, 0.0f);
				return 1;
			}

			int _c_c_set_active(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L);
				if (camera != nullptr)
					common::renderer->set_active_camera(camera);
				return 0;
			}

			int _c_c_get_active(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L);
				lua_pushboolean(L, common::renderer->get_active_camera() == camera);
				return 1;
			}

			/*
				Behavior
			*/

			int _c_b_set_behavior(lua_State* L)
			{
				auto ptr = load_component<::entities::components::behavior>(L);
				auto asset = load_id(L, 3);
				if (ptr != nullptr)
					ptr->behavior_asset = ::assets::cast_asset<::assets::behavior>(::common::assets_manager->get_asset(asset)).lock();

				return 0;
			}

			/*
				Colider
			*/

			int _c_cl_set_collision_preset(lua_State* L)
			{
				auto ptr = load_component<::entities::components::collider>(L);
				auto preset = lua_tostring(L, 3);

				auto config = ::assets::cast_asset<::assets::collision_config>(::common::assets_manager->get_asset(utilities::hash_string("mod/collision_config")));

				if (ptr != nullptr)
					ptr->preset = config.lock()->get_preset(utilities::hash_string(preset));
				return 0;
			}

			int _c_cl_set_entity_offset(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L);
				cl->entity_offset.x = lua_tonumber(L, 3);
				cl->entity_offset.y = lua_tonumber(L, 4);
				return 0;
			}

			int _c_cl_get_entity_offset(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L);
				lua_pushnumber(L, cl->entity_offset.x);
				lua_pushnumber(L, cl->entity_offset.y);
				return 2;
			}

			/*
				Register
			*/

			void register_functions(lua_State* L)
			{
				lua_register(L, "_c_g_set_entity_offset", _c_g_set_entity_offset);
				lua_register(L, "_c_g_get_entity_offset", _c_g_get_entity_offset);

				lua_register(L, "_c_c_set_ortho_width", _c_c_set_ortho_width);
				lua_register(L, "_c_c_get_ortho_width", _c_c_get_ortho_width);
				lua_register(L, "_c_c_set_active", _c_c_set_active);
				lua_register(L, "_c_c_get_active", _c_c_get_active);

				lua_register(L, "_c_b_set_behavior", _c_b_set_behavior);

				lua_register(L, "_c_cl_set_collision_preset", _c_cl_set_collision_preset);
				lua_register(L, "_c_cl_set_entity_offset", _c_cl_set_entity_offset);
				lua_register(L, "_c_cl_get_entity_offset", _c_cl_get_entity_offset);
			}
		}
	}
}