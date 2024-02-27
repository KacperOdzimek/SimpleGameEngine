#include "entities_functions.h"

#include "utilities.h"

#include "source/components/behavior.h"

#include "source/common/common.h"
#include "source/behaviors/behaviors_manager.h"
#include "source/entities/world.h"

#include "source/behaviors/frame.h"

namespace behaviors
{
	namespace lua_functions
	{
#define scene common::behaviors_manager->get_current_frame()->scene_context->get()
		namespace entities
		{
			int _e_create(lua_State* L)
			{
				auto e = new ::entities::entity;
				lua_pushinteger(L, (uint64_t)&scene->entities.back());
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
				auto e = reinterpret_cast<std::weak_ptr<::entities::entity>*>(lua_tointeger(L, 1));
				lua_pushboolean(L, !e->expired());
				return 1;
			}

			int _e_call(lua_State* L)
			{
				auto e = load_entity(L, 1, "[_e_call]");
				const char* name = lua_tostring(L, 2);
				lua_remove(L, 2);
				e->call_event(name);
				return 0;
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
				int layer = lua_tointeger(L, 2);
				e->layer = layer;
				return 0;
			}

			void register_functions(lua_State* L)
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