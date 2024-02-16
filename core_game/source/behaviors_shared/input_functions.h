class lua_State;

#include "source/common/common.h"
#include "source/input/input_manager.h"

namespace behaviors
{
	namespace lua_functions
	{
		namespace input
		{
			int _i_action(lua_State* L);
			int _i_axis(lua_State* L);
			void register_functions(lua_State* L);
		}
	}
}