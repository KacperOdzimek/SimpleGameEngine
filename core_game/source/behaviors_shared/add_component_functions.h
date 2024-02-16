class lua_State;

namespace behaviors
{
	namespace lua_functions
	{
		namespace add_component
		{
			int _e_add_behavior(lua_State* L);
			int _e_add_camera(lua_State* L);
			int _e_add_mesh(lua_State* L);
			void register_functions(lua_State* L);
		}
	}
}