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
#include "source/components/static_mesh.h"
#include "source/components/sprite.h"
#include "source/components/flipbook.h"
#include "source/components/dynamics.h"

namespace behaviors
{
	namespace lua_shared
	{
		namespace components
		{
			/*
				Any Mesh
			*/

			int _c_m_get_scale(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L, "[_c_m_get_scale]");
				lua_pushnumber(L, mesh->get_scale().x);
				lua_pushnumber(L, mesh->get_scale().y);
				return 2;
			}

			int _c_m_set_scale(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L, "[_c_m_set_scale]");
				float x = lua_tonumber(L, 3);
				float y = lua_tonumber(L, 4);
				mesh->set_scale({ x, y });
				return 0;
			}

			int _c_m_get_offset(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L, "[_c_m_get_offset]");
				lua_pushnumber(L, mesh->get_offset().x);
				lua_pushnumber(L, mesh->get_offset().y);
				return 2;
			}

			int _c_m_set_offset(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L, "[_c_m_set_offset]");
				float x = lua_tonumber(L, 3);
				float y = lua_tonumber(L, 4);
				mesh->set_offset({ x, y });
				return 0;
			}

			int _c_m_get_visible(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L, "[_c_m_get_visible]");
				lua_pushboolean(L, mesh->get_visible());
				return 1;
			}

			int _c_m_set_visible(lua_State* L)
			{
				auto mesh = load_component<::entities::components::mesh>(L, "[_c_m_set_visible]");
				mesh->set_visible(lua_toboolean(L, 3));
				return 0;
			}

			/*
				Static Mesh
			*/

			int _c_sm_get_render_config(lua_State* L)
			{
				auto mesh = load_component<::entities::components::static_mesh>(L, "[_c_sm_get_render_config]");
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

			int _c_sm_set_render_config(lua_State* L)
			{
				auto mesh = load_component<::entities::components::static_mesh>(L, "[_c_sm_set_render_config]");
				rendering::render_config rc = load_render_config(L, 3, "[_c_sm_set_render_config]");

				mesh->set_render_config(rc);

				return 0;
			}

			/*
				Sprite
			*/

			int _c_s_get_sprite(lua_State* L)
			{
				auto sprite = load_component<::entities::components::sprite>(L, "[_c_s_set_sprite]");

				lua_pushinteger(L, sprite->get_sprite_id());

				return 0;
			}

			int _c_s_set_sprite(lua_State* L)
			{
				auto sprite = load_component<::entities::components::sprite>(L, "[_c_s_set_sprite]");
				int sprite_id = lua_tointeger(L, 3);

				sprite->set_sprite_id(sprite_id);

				return 0;
			}

			/*
				Flipbook
			*/

			int _c_f_get_animation(lua_State* L)
			{
				auto flipbook = load_component<::entities::components::flipbook>(L, "[_c_f_get_animation]");

				lua_pushinteger(L, flipbook->get_animation());

				return 0;
			}

			int _c_f_set_animation(lua_State* L)
			{
				auto flipbook = load_component<::entities::components::flipbook>(L, "[_c_f_set_animation]");
				uint32_t animation = load_id(L, 3, "[_c_f_set_animation]", "Animation");

				flipbook->set_animation(animation);

				return 0;
			}

			int _c_f_get_looping(lua_State* L)
			{
				auto flipbook = load_component<::entities::components::flipbook>(L, "[_c_f_get_animation]");

				lua_pushboolean(L, flipbook->looping);

				return 0;
			}

			int _c_f_set_looping(lua_State* L)
			{
				auto flipbook = load_component<::entities::components::flipbook>(L, "[_c_f_set_animation]");
				bool looping = lua_toboolean(L, 3);

				flipbook->looping = looping;

				return 0;
			}

			/*
				Camera
			*/

			int _c_c_get_ortho_width(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L, "[_c_c_get_ortho_width]");
				if (camera != nullptr)
					lua_pushnumber(L, camera->ortho_width);
				else
					lua_pushnumber(L, 0.0f);
				return 1;
			}

			int _c_c_set_ortho_width(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L, "[_c_c_set_ortho_width]");
				float width = lua_tonumber(L, 3);
				if (camera != nullptr)
					camera->ortho_width = width;
				return 0;
			}

			int _c_c_get_lowest_layer(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L, "[_c_c_get_rendered_layers]");
				lua_pushinteger(L, camera->lowest_layer);
				return 1;
			}

			int _c_c_set_lowest_layer(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L, "[_c_c_set_rendered_layers]");
				camera->lowest_layer = lua_tonumber(L, 3);
				return 0;
			}

			int _c_c_get_highest_layer(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L, "[_c_c_get_rendered_layers]");
				lua_pushinteger(L, camera->highest_layer);
				return 1;
			}

			int _c_c_set_highest_layer(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L, "[_c_c_set_rendered_layers]");
				camera->highest_layer = lua_tonumber(L, 3);
				return 0;
			}

			int _c_c_get_active(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L, "[_c_c_get_active]");
				lua_pushboolean(L, common::renderer->get_active_camera() == camera);
				return 1;
			}

			int _c_c_set_active(lua_State* L)
			{
				auto camera = load_component<::entities::components::camera>(L, "[_c_c_set_active]");
				common::renderer->set_active_camera(camera);
				return 0;
			}

			/*
				Behavior
			*/

			int _c_b_get_behavior(lua_State* L)
			{
				auto ptr = load_component<::entities::components::behavior>(L, "[_c_b_get_behavior]");
				lua_pushstring(L, ptr->behavior_asset->package_name.c_str());
				return 1;
			}

			int _c_b_set_behavior(lua_State* L)
			{
				auto ptr = load_component<::entities::components::behavior>(L, "[_c_b_set_behavior]");
				auto asset = load_asset_path(L, 3, "[_c_b_set_behavior]");
				if (ptr != nullptr)
					ptr->behavior_asset = ::assets::cast_asset<::assets::behavior>(::common::assets_manager->safe_get_asset(asset)).lock();
				return 0;
			}
			
			int _c_b_call(lua_State* L)
			{
				auto ptr = load_component<::entities::components::behavior>(L, "[_c_b_call]");
				const char* name = lua_tostring(L, 3);

				lua_remove(L, 2);	//Remove component id
				lua_remove(L, 2);	//Remove event name

				std::string event_name = "event_";
				event_name += name;

				ptr->call_custom_function(event_name);

				return 0;
			}

			/*
				Collider
			*/

			int _c_cl_get_collision_preset(lua_State* L)
			{
				auto ptr = load_component<::entities::components::collider>(L, "[_c_cl_get_collision_preset]");
				auto config = ::assets::cast_asset<::assets::collision_config>(::common::assets_manager->get_asset(
					utilities::hash_string("mod/collision_config"))).lock();

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
				auto ptr = load_component<::entities::components::collider>(L, "[_c_cl_set_collision_preset]");
				auto preset = lua_tostring(L, 3);

				auto config = ::assets::cast_asset<::assets::collision_config>(
					::common::assets_manager->get_asset(utilities::hash_string("mod/collision_config")));

				if (ptr != nullptr)
					ptr->preset = config.lock()->get_preset(utilities::hash_string(preset));
				return 0;
			}

			int _c_cl_get_offset(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L, "[_c_cl_get_offset]");
				lua_pushnumber(L, cl->entity_offset.x);
				lua_pushnumber(L, cl->entity_offset.y);
				return 2;
			}

			int _c_cl_set_offset(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L, "[_c_cl_set_offset]");
				cl->entity_offset.x = lua_tonumber(L, 3);
				cl->entity_offset.y = lua_tonumber(L, 4);
				return 0;
			}

			int _c_cl_get_extend(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L, "[_c_cl_get_extend]");
				lua_pushnumber(L, cl->extend.x);
				lua_pushnumber(L, cl->extend.y);
				return 2;
			}

			int _c_cl_set_extend(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L, "[_c_cl_set_extend]");
				cl->extend.x = lua_tonumber(L, 3);
				cl->extend.y = lua_tonumber(L, 4);
				return 0;
			}

			int _c_cl_get_layer_offset(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L, "[_c_cl_get_layer_offset]");
				lua_pushinteger(L, cl->layer_offset);
				return 1;
			}

			int _c_cl_set_layer_offset(lua_State* L)
			{
				auto cl = load_component<::entities::components::collider>(L, "[_c_cl_set_layer_offset]");
				cl->layer_offset = lua_tointeger(L, 3);
				return 0;
			}

			/*
				Dynamics
			*/

			int _c_d_add_force(lua_State* L)
			{
				auto d = load_component<::entities::components::dynamics>(L, "[_c_d_add_force]");
				float x = lua_tonumber(L, 3);
				float y = lua_tonumber(L, 4);
				d->add_force({ x, y });
				return 0;
			}

			int _c_d_add_movement_input(lua_State* L)
			{
				auto d = load_component<::entities::components::dynamics>(L, "[_c_d_add_movement_force]");

				float dir_x = lua_tonumber(L, 3);
				float dir_y = lua_tonumber(L, 4);
				float speed = lua_tonumber(L, 5);

				glm::vec2 vec = { dir_x, dir_y };

				if (glm::length(vec) == 0.0f)
					return 0;

				vec = glm::normalize(vec) * speed;
				d->add_force(vec);

				return 0;
			}

			int _c_d_get_drag(lua_State* L)
			{
				auto d = load_component<::entities::components::dynamics>(L, "[_c_d_get_drag]");
				lua_pushnumber(L, d->drag);
				return 1;
			}

			int _c_d_set_drag(lua_State* L)
			{
				auto d = load_component<::entities::components::dynamics>(L, "[_c_d_set_drag]");
				auto drag = lua_tonumber(L, 3);
				d->drag = drag;
				return 0;
			}

			int _c_d_get_mass(lua_State* L)
			{
				auto d = load_component<::entities::components::dynamics>(L, "[_c_d_get_mass]");
				lua_pushnumber(L, d->mass);
				return 1;
			}

			int _c_d_set_mass(lua_State* L)
			{
				auto d = load_component<::entities::components::dynamics>(L, "[_c_d_set_mass]");
				auto mass = lua_tonumber(L, 3);
				d->mass = mass;
				return 0;
			}

			int _c_d_get_use_max_vel(lua_State* L)
			{
				auto d = load_component<::entities::components::dynamics>(L, "[_c_d_get_use_max_vel]");
				lua_pushboolean(L, d->use_maximum_velocity);
				return 1;
			}

			int _c_d_set_use_max_vel(lua_State* L)
			{
				auto d = load_component<::entities::components::dynamics>(L, "[_c_d_set_use_max_vel]");
				auto use = lua_toboolean(L, 3);
				d->use_maximum_velocity = use;
				return 0;
			}

			int _c_d_get_max_vel(lua_State* L)
			{
				auto d = load_component<::entities::components::dynamics>(L, "[_c_d_get_max_vel]");
				lua_pushnumber(L, d->maximum_velocity);
				return 1;
			}

			int _c_d_set_max_vel(lua_State* L)
			{
				auto d = load_component<::entities::components::dynamics>(L, "[_c_d_set_max_vel]");
				auto vel = lua_tonumber(L, 3);
				d->maximum_velocity = vel;
				return 0;
			}

			/*
				Register
			*/

			void register_shared(lua_State* L)
			{
				lua_register(L, "_c_m_get_visible", _c_m_get_visible);
				lua_register(L, "_c_m_set_visible", _c_m_set_visible);
				lua_register(L, "_c_m_get_scale", _c_m_get_scale);
				lua_register(L, "_c_m_set_scale", _c_m_set_scale);
				lua_register(L, "_c_m_set_offset", _c_m_set_offset);
				lua_register(L, "_c_m_get_offset", _c_m_get_offset);

				lua_register(L, "_c_sm_get_render_config", _c_sm_get_render_config);
				lua_register(L, "_c_sm_set_render_config", _c_sm_set_render_config);

				lua_register(L, "_c_s_get_sprite", _c_s_get_sprite);
				lua_register(L, "_c_s_set_sprite", _c_s_set_sprite);

				lua_register(L, "_c_f_get_animation", _c_f_get_animation);
				lua_register(L, "_c_f_set_animation", _c_f_set_animation);
				lua_register(L, "_c_f_get_looping", _c_f_get_looping);
				lua_register(L, "_c_f_set_looping", _c_f_set_looping);

				lua_register(L, "_c_c_get_ortho_width", _c_c_get_ortho_width);
				lua_register(L, "_c_c_set_ortho_width", _c_c_set_ortho_width);
				lua_register(L, "_c_c_get_lowest_layer", _c_c_get_lowest_layer);
				lua_register(L, "_c_c_set_lowest_layer", _c_c_set_lowest_layer);
				lua_register(L, "_c_c_get_highest_layer", _c_c_get_highest_layer);
				lua_register(L, "_c_c_set_highest_layer", _c_c_set_highest_layer);
				lua_register(L, "_c_c_get_active", _c_c_get_active);
				lua_register(L, "_c_c_set_active", _c_c_set_active);
				
				lua_register(L, "_c_b_get_behavior", _c_b_get_behavior);
				lua_register(L, "_c_b_set_behavior", _c_b_set_behavior);
				lua_register(L, "_c_b_call", _c_b_call);

				lua_register(L, "_c_cl_get_collision_preset", _c_cl_get_collision_preset);
				lua_register(L, "_c_cl_set_collision_preset", _c_cl_set_collision_preset);
				lua_register(L, "_c_cl_get_offset", _c_cl_get_offset);
				lua_register(L, "_c_cl_set_offset", _c_cl_set_offset);
				lua_register(L, "_c_cl_get_extend", _c_cl_get_extend);
				lua_register(L, "_c_cl_set_extend", _c_cl_set_extend);
				lua_register(L, "_c_cl_get_layer_offset", _c_cl_get_layer_offset);
				lua_register(L, "_c_cl_set_layer_offset", _c_cl_set_layer_offset);

				lua_register(L, "_c_d_add_force", _c_d_add_force);
				lua_register(L, "_c_d_add_movement_input", _c_d_add_movement_input);
				lua_register(L, "_c_d_get_drag", _c_d_get_drag);
				lua_register(L, "_c_d_set_drag", _c_d_set_drag);
				lua_register(L, "_c_d_get_mass", _c_d_get_mass);
				lua_register(L, "_c_d_set_mass", _c_d_set_mass);
				lua_register(L, "_c_d_get_use_max_vel", _c_d_get_use_max_vel);
				lua_register(L, "_c_d_set_use_max_vel", _c_d_set_use_max_vel);
				lua_register(L, "_c_d_get_max_vel", _c_d_get_max_vel);
				lua_register(L, "_c_d_set_max_vel", _c_d_set_max_vel);
			}
		}
	}
}