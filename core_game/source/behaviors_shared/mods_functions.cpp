#include "collision_functions.h"

#include "utilities.h"

#include "source/common/common.h"

#include "source/mods/mods_manager.h"

namespace behaviors
{
	namespace lua_shared
	{
		namespace mods
		{
			int _m_quit_mod(lua_State* L)
			{
				common::state = common::program_state::pending_for_mod_quit;

				return 0;
			}

			int _m_load_mod(lua_State* L)
			{
				std::string mod = lua_tostring(L, 1);
				
				common::state = common::program_state::pending_for_mod_load;
				common::state_info = mod;

				return 0;
			}

			int _m_get_all_mods(lua_State* L)
			{
				auto mods = common::mods_manager->get_all_mods();

				lua_newtable(L);

				for (int i = 0; i < mods.size(); i++)
				{
					lua_pushinteger(L, i);
					lua_pushstring(L, mods.at(i).c_str());
					lua_settable(L, -3);
				}

				return 1;
			}

			void register_shared(lua_State* L)
			{
				lua_register(L, "_m_quit_mod", _m_quit_mod);
				lua_register(L, "_m_load_mod", _m_load_mod);
				lua_register(L, "_m_get_all_mods", _m_get_all_mods);
			}
		}
	}
}