namespace behaviors
{
	namespace lua_functions
	{
		namespace databases
		{
#define db internal::active_database
			int _d_set_f(lua_State* L)
			{
				auto id = load_id(L, 1);
				auto val = lua_tonumber(L, 2);
				db->set_float(id, val);
				return 0;
			}

			int _d_get(lua_State* L)
			{
				auto id = load_id(L, 1);
				lua_pushnumber(L, db->get_float(id));
				return 1;
			}

			void register_functions(lua_State* L)
			{
				lua_register(L, "_d_set_f", _d_set_f);
				lua_register(L, "_d_get", _d_get);
			}
#undef db
		}
	}
}