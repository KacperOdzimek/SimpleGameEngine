class lua_State;

#include "source/common/common.h"
#include "source/input/input_manager.h"

namespace behaviors
{
	namespace lua_functions
	{
		namespace input
		{
			void register_functions(lua_State* L);
		}
	}
}