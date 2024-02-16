class lua_State;

namespace behaviors
{
	namespace lua_functions
	{
		namespace components
		{
			/*
				Mesh
			*/

			/*
				Camera
			*/
			int _c_c_set_ortho_width(lua_State* L);
			int _c_c_get_ortho_width(lua_State* L);
			int _c_c_set_active(lua_State* L);
			int _c_c_get_active(lua_State* L);

			/*
				Behavior
			*/
			int _c_b_set_behavior(lua_State* L);

			/*
				Colider
			*/
			int _c_cl_set_collision_preset(lua_State* L);
			int _c_cl_set_entity_offset(lua_State* L);
			int _c_cl_get_entity_offset(lua_State* L);

			/*
				Register
			*/
			void register_functions(lua_State* L);
		}
	}
}