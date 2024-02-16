class lua_State;

namespace behaviors
{
	namespace lua_functions
	{
		namespace entities
		{
			int _e_create(lua_State* L);
			int _e_kill(lua_State* L);
			int _e_is_valid(lua_State* L);
			int _e_call(lua_State* L);
			int _e_teleport(lua_State* L);
			int _e_sweep(lua_State* L);
			int _e_get_location(lua_State* L);
			int _e_kill_component(lua_State* L);
			void register_functions(lua_State* L);
		}
	}
}