#include "components_functions.h"

#include "utilities.h"

#include "source/entities/entity.h"
#include "source/utilities/hash_string.h"

#include "source/assets/assets_manager.h"
#include "source/rendering/renderer.h"

#include "source/components/behavior.h"

#include "source/assets/asset.h"
#include "source/assets/behavior_asset.h"
#include "source/assets/collision_config_asset.h"

#include "source/components/camera.h"
#include "source/components/behavior.h"
#include "source/components/collider.h"
#include "source/components/mesh.h"

namespace behaviors
{
	namespace lua_functions
	{
		namespace components
		{
			/*
				Mesh
			*/

			int _c_m_get_render_config(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L);
				auto& rc = mesh->get_render_config();

				lua_createtable(L, 0, 0);

				lua_pushstring(L, "shader");
				lua_pushstring(L, rc.material->package_name.c_str());
				lua_settable(L, -3);

				lua_pushstring(L, "mesh");
				lua_pushstring(L, rc.mesh->package_name.c_str());
				lua_settable(L, -3);

				lua_pushstring(L, "textures");
				lua_createtable(L, 0, 0);

				for (int i = 1; i < rc.textures.size() + 1; i++)
				{
					lua_pushinteger(L, 1);
					lua_pushstring(L, rc.textures.at(i - 1)->package_name.c_str());
					lua_settable(L, -3);
				}

				lua_settable(L, -3);

				return 1;
			}

			int _c_m_set_render_config(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L);
				rendering::render_config rc = load_render_config(L, 3);

				mesh->set_render_config(rc);

				return 0;
			}

			int _c_m_get_visible(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L);
				lua_pushboolean(L, mesh->visible);
				return 1;
			}

			int _c_m_set_visible(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L);
				mesh->visible = lua_toboolean(L, 3);
				return 0;
			}

			int _c_m_get_scale(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L);
				lua_pushnumber(L, mesh->scale.x);
				lua_pushnumber(L, mesh->scale.y);
				return 2;
			}

			int _c_m_set_scale(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L);
				float x = lua_tonumber(L, 3);
				float y = lua_tonumber(L, 4);
				mesh->scale = { x, y };
				return 0;
			}

			int _c_m_get_offset(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L);
				lua_pushnumber(L, mesh->offset.x);
				lua_pushnumber(L, mesh->offset.y);
				return 2;
			}

			int _c_m_set_offset(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L);
				float x = lua_tonumber(L, 3);
				float y = lua_tonumber(L, 4);
				mesh->offset = { x, y };
				return 0;
			}

			/*
				Camera
			*/

			int _c_c_get_ortho_width(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L);
				if (camera != nullptr)
					lua_pushnumber(L, camera->ortho_width);
				else
					lua_pushnumber(L, 0.0f);
				return 1;
			}

			int _c_c_set_ortho_width(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L);
				float width = lua_tonumber(L, 3);
				if (camera != nullptr)
					camera->ortho_width = width;
				return 0;
			}

			int _c_c_get_rendered_layers(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L);
				lua_pushinteger(L, camera->rendered_layers);
				return 1;
			}

			int _c_c_set_rendered_layers(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L);
				camera->rendered_layers = lua_tonumber(L, 3);
				return 0;
			}

			int _c_c_get_active(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L);
				lua_pushboolean(L, common::renderer->get_active_camera() == camera);
				return 1;
			}

			int _c_c_set_active(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L);
				if (camera != nullptr)
					common::renderer->set_active_camera(camera);
				return 0;
			}

			/*
				Behavior
			*/

			int _c_b_get_behavior(lua_State* L)
			{
				auto ptr = load_component<::entities::components::behavior>(L);
				lua_pushstring(L, ptr->behavior_asset->package_name.c_str());
				return 1;
			}

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

			int _c_cl_get_collision_preset(lua_State* L)
			{
				auto ptr = load_component<::entities::components::collider>(L);
				auto config = ::assets::cast_asset<::assets::collision_config>(::common::assets_manager->get_asset(utilities::hash_string("mod/collision_config"))).lock();

				auto it = std::find_if(config->collision_presets.begin(), config->collision_presets.end(),
					[&ptr](auto&& p) { return p.second == ptr->preset; });

				if (it == config->collision_presets.end())
					lua_pushinteger(L, ptr->preset);
				else
					lua_pushstring(L, config->collision_presets_names.at(it->first).c_str());

				return 1;
			}

			int _c_cl_set_collision_preset(lua_State* L)
			{
				auto ptr = load_component<::entities::components::collider>(L);
				auto preset = lua_tostring(L, 3);

				auto config = ::assets::cast_asset<::assets::collision_config>(::common::assets_manager->get_asset(utilities::hash_string("mod/collision_config")));

				if (ptr != nullptr)
					ptr->preset = config.lock()->get_preset(utilities::hash_string(preset));
				return 0;
			}

			int _c_cl_get_offset(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L);
				lua_pushnumber(L, cl->entity_offset.x);
				lua_pushnumber(L, cl->entity_offset.y);
				return 2;
			}

			int _c_cl_set_offset(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L);
				cl->entity_offset.x = lua_tonumber(L, 3);
				cl->entity_offset.y = lua_tonumber(L, 4);
				return 0;
			}

			int _c_cl_get_extend(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L);
				lua_pushnumber(L, cl->extend.x);
				lua_pushnumber(L, cl->extend.y);
				return 2;
			}

			int _c_cl_set_extend(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L);
				cl->extend.x = lua_tonumber(L, 3);
				cl->extend.y = lua_tonumber(L, 4);
				return 0;
			}

			int _c_cl_get_layer_offset(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L);
				lua_pushinteger(L, cl->layer_offset);
				return 1;
			}

			int _c_cl_set_layer_offset(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L);
				cl->layer_offset = lua_tointeger(L, 3);
				return 0;
			}

			/*
				Register
			*/

			void register_functions(lua_State* L)
			{
				lua_register(L, "_c_m_get_render_config", _c_m_get_render_config);
				lua_register(L, "_c_m_set_render_config", _c_m_set_render_config);
				lua_register(L, "_c_m_get_visible", _c_m_get_visible);
				lua_register(L, "_c_m_set_visible", _c_m_set_visible);
				lua_register(L, "_c_m_get_scale", _c_m_get_scale);
				lua_register(L, "_c_m_set_scale", _c_m_set_scale);
				lua_register(L, "_c_m_set_offset", _c_m_set_offset);
				lua_register(L, "_c_m_get_offset", _c_m_get_offset);

				lua_register(L, "_c_c_get_ortho_width", _c_c_get_ortho_width);
				lua_register(L, "_c_c_set_ortho_width", _c_c_set_ortho_width);
				lua_register(L, "_c_c_get_rendered_layers", _c_c_get_rendered_layers);
				lua_register(L, "_c_c_set_rendered_layers", _c_c_set_rendered_layers);
				lua_register(L, "_c_c_get_active", _c_c_get_active);
				lua_register(L, "_c_c_set_active", _c_c_set_active);

				lua_register(L, "_c_b_get_behavior", _c_b_get_behavior);
				lua_register(L, "_c_b_set_behavior", _c_b_set_behavior);

				lua_register(L, "_c_cl_get_collision_preset", _c_cl_get_collision_preset);
				lua_register(L, "_c_cl_set_collision_preset", _c_cl_set_collision_preset);
				lua_register(L, "_c_cl_get_offset", _c_cl_get_offset);
				lua_register(L, "_c_cl_set_offset", _c_cl_set_offset);
				lua_register(L, "_c_cl_get_extend", _c_cl_get_extend);
				lua_register(L, "_c_cl_set_extend", _c_cl_set_extend);
				lua_register(L, "_c_cl_get_layer_offset", _c_cl_get_layer_offset);
				lua_register(L, "_c_cl_set_layer_offset", _c_cl_set_layer_offset);
			}
		}
	}
}