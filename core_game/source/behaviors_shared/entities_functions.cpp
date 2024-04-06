#include "entities_functions.h"

#include "utilities.h"

#include "source/entities/component.h"
#include "source/components/behavior.h"

#include "source/common/common.h"
#include "source/behaviors/behaviors_manager.h"
#include "source/entities/world.h"

#include "source/behaviors/frame.h"

namespace behaviors
{
	namespace lua_shared
	{
#define scene common::behaviors_manager->get_current_frame()->scene_context
		namespace entities
		{
			int _e_create(lua_State* L)
			{
				auto e = new ::entities::entity{scene};
				push_entity(L, scene->entities.back());
				return 1;
			}

			int _e_kill(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_kill]");
				e->kill();
				return 0;
			}

			int _e_is_alive(lua_State* L)
			{
				if (!lua_isuserdata(L, 1))
					error_handling::crash(error_handling::error_source::mod, "[_e_is_alive]",
						"Entity reference should be std::weak_ptr<::entities::entity>* userdata");

				auto* e = reinterpret_cast<std::weak_ptr<::entities::entity>*>(luaL_checkudata(L, 1, "entity"));
				luaL_argcheck(L, e != NULL, 1, "Entity reference expected");

				lua_pushboolean(L, !e->expired());
					
				return 1;
			}

			int _e_call(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_call]");
				const char* name = lua_tostring(L, 2);
				lua_remove(L, 1);
				lua_remove(L, 1);
				int args = luaL_ref(L, LUA_REGISTRYINDEX);

				auto& comps = e->get_components();
				std::string event_name = "event_";
				event_name += name;

				lua_newtable(L);

				int result_table = luaL_ref(L, LUA_REGISTRYINDEX);

				int counter = 1;
				for (auto& comp : comps)
				{
					auto behavior = dynamic_cast<::entities::components::behavior*>(comp);
					if (behavior != nullptr)
					{
						behavior->call_custom_function(event_name, args);
						lua_rawgeti(L, LUA_REGISTRYINDEX, result_table);
						lua_pushinteger(L, counter);
						lua_insert(L, -3);
						lua_insert(L, -3);
						lua_settable(L, -3);
						lua_remove(L, -1);
						counter++;
					}
				}		

				luaL_unref(L, LUA_REGISTRYINDEX, args);
				lua_rawgeti(L, LUA_REGISTRYINDEX, result_table);
				luaL_unref(L, LUA_REGISTRYINDEX, result_table);
				return 1;
			}

			int _e_teleport(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_teleport]");
				float x = (float)lua_tonumber(L, 2);
				float y = (float)lua_tonumber(L, 3);
				e->teleport({ x, y });
				return 0;
			}

			int _e_sweep(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_sweep]");
				float x = (float)lua_tonumber(L, 2);
				float y = (float)lua_tonumber(L, 3);
				auto event = e->sweep({ x, y });
				lua_pushboolean(L, event.response == physics::collision_response::collide);
				return 1;
			}

			int _e_get_location(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_get_location]");
				lua_pushnumber(L, e->get_location().x);
				lua_pushnumber(L, e->get_location().y);
				return 2;
			}

			int _e_kill_component(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_kill_component]");
				uint32_t id = load_id(L, 2, "[_e_kill_component]", "Component");
				e->kill_component(id);
				return 0;
			}

			int _e_get_layer(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_set_layer]");
				lua_pushinteger(L, e->layer);
				return 1;
			}

			int _e_set_layer(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_set_layer]");
				int layer = static_cast<int>(lua_tointeger(L, 2));
				e->layer = layer;
				return 0;
			}

			void register_shared(lua_State* L)
			{
				lua_register(L, "_e_create", _e_create);
				lua_register(L, "_e_kill", _e_kill);
				lua_register(L, "_e_is_alive", _e_is_alive);
				lua_register(L, "_e_call", _e_call);
				lua_register(L, "_e_teleport", _e_teleport);
				lua_register(L, "_e_sweep", _e_sweep);
				lua_register(L, "_e_get_location", _e_get_location);
				lua_register(L, "_e_kill_component", _e_kill_component);
				lua_register(L, "_e_get_layer", _e_get_layer);
				lua_register(L, "_e_set_layer", _e_set_layer);
			}
#undef scene
		}
	}
}