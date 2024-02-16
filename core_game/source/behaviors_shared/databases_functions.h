class lua_State;

namespace behaviors
{
	namespace lua_functions
	{
		namespace databases
		{
			int _d_set_f(lua_State* L);
			int _d_set_i(lua_State* L);
			int _d_set_b(lua_State* L);
			int _d_set_s(lua_State* L);
			int _d_set_e(lua_State* L);
			int _d_get(lua_State* L);
			void register_functions(lua_State* L);
		}
	}
}