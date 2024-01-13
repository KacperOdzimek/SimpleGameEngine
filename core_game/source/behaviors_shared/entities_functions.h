namespace behaviors
{
	namespace lua_functions
	{
		namespace entities
		{
			int _e_create(lua_State* L)
			{
				auto ent = new ::entities::entity;
				lua_pushinteger(L, (uint64_t)ent);
				return 1;
			}

			int _e_kill(lua_State* L)
			{
				auto pointer = load_entity(L, 1);
				pointer->kill();
				return 0;
			}

			int _e_set_location(lua_State* L)
			{
				auto pointer = load_entity(L, 1);
				float x = (float)lua_tonumber(L, 2);
				float y = (float)lua_tonumber(L, 3);
				pointer->position = { x, y };
				return 0;
			}

			int _e_get_location(lua_State* L)
			{
				auto pointer = load_entity(L, 1);
				lua_pushnumber(L, pointer->position.x);
				lua_pushnumber(L, pointer->position.y);
				return 2;
			}

			int _e_kill_component(lua_State* L)
			{
				auto pointer = load_entity(L, 1);
				uint32_t id = load_id(L, 2);
				pointer->kill_component(id);
				return 0;
			}

			void register_functions(lua_State* L)
			{
				lua_register(L, "_e_create", _e_create);
				lua_register(L, "_e_kill", _e_kill);
				lua_register(L, "_e_set_location", _e_set_location);
				lua_register(L, "_e_get_location", _e_get_location);
				lua_register(L, "_e_kill_component", _e_kill_component);
			}
		}
	}
}