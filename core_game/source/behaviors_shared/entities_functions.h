namespace behaviors
{
	namespace lua_functions
	{
		namespace entities
		{
			int _e_create(lua_State* L)
			{
				auto e = new ::entities::entity;
				lua_pushinteger(L, (uint64_t)e);
				return 1;
			}

			int _e_kill(lua_State* L)
			{
				auto e = load_entity(L, 1);
				e->kill();
				return 0;
			}

			int _is_e_valid(lua_State* L)
			{
				auto e = reinterpret_cast<std::weak_ptr<::entities::entity>*>(lua_tointeger(L, 1));
				lua_pushboolean(L, !e->expired());
				return 1;
			}

			int _e_set_location(lua_State* L)
			{
				auto e = load_entity(L, 1);
				float x = (float)lua_tonumber(L, 2);
				float y = (float)lua_tonumber(L, 3);
				e->position = { x, y };
				return 0;
			}

			int _e_get_location(lua_State* L)
			{
				auto e = load_entity(L, 1);
				lua_pushnumber(L, e->position.x);
				lua_pushnumber(L, e->position.y);
				return 2;
			}

			int _e_kill_component(lua_State* L)
			{
				auto e = load_entity(L, 1);
				uint32_t id = load_id(L, 2);
				e->kill_component(id);
				return 0;
			}

			void register_functions(lua_State* L)
			{
				lua_register(L, "_e_create", _e_create);
				lua_register(L, "_e_kill", _e_kill);
				lua_register(L, "_is_e_valid", _is_e_valid);
				lua_register(L, "_e_set_location", _e_set_location);
				lua_register(L, "_e_get_location", _e_get_location);
				lua_register(L, "_e_kill_component", _e_kill_component);
			}
		}
	}
}