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

			int _d_set_i(lua_State* L)
			{
				auto id = load_id(L, 1);
				auto val = lua_tointeger(L, 2);
				db->set_int(id, val);
				return 0;
			}

			int _d_set_s(lua_State* L)
			{
				auto id = load_id(L, 1);
				auto val = lua_tostring(L, 2);
				db->set_string(id, val);
				return 0;
			}

			int _d_set_e(lua_State* L)
			{
				auto id = load_id(L, 1);
				auto val = reinterpret_cast<std::weak_ptr<::entities::entity>*>(lua_tointeger(L, 2));
				db->set_entity(id, val);
				return 0;
			}

			int _d_get(lua_State* L)
			{
				auto id = load_id(L, 1);
				switch (db->get_data_type_at(id))
				{
				case behaviors::database_data_types::unspecified:
					return 0;
				case behaviors::database_data_types::Float:
					lua_pushnumber(L, db->get_float(id));
					return 1;
				case behaviors::database_data_types::Int:
					lua_pushinteger(L, db->get_int(id));
					return 1;
				case behaviors::database_data_types::string:
					lua_pushstring(L, db->get_string(id).c_str());
					return 1;	
				case behaviors::database_data_types::entity:
					auto x = db->get_entity(id);
					lua_pushinteger(L, (uint64_t)x);
					return 1;
				}	
				return 0;
			}

			void register_functions(lua_State* L)
			{
				lua_register(L, "_d_set_f", _d_set_f);
				lua_register(L, "_d_set_i", _d_set_i);
				lua_register(L, "_d_set_s", _d_set_s);
				lua_register(L, "_d_set_e", _d_set_e);
				lua_register(L, "_d_get", _d_get);
			}
#undef db
		}
	}
}