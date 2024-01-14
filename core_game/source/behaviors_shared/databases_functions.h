namespace behaviors
{
	namespace lua_functions
	{
		namespace databases
		{
			int _d_set_float(lua_State* L)
			{
				auto db = load_database(L, 1);
				auto id = load_id(L, 2);
				db->set_float(id, lua_tonumber(L, 3));
				return 0;
			}

			int _d_get_float(lua_State* L)
			{
				auto db = load_database(L, 1);
				auto id = load_id(L, 2);
				lua_pushnumber(L, db->get_float(id));
				return 1;
			}

			void register_functions(lua_State* L)
			{
				lua_register(L, "_d_set_float", _d_set_float);
				lua_register(L, "_d_get_float", _d_get_float);
			}
		}
	}
}