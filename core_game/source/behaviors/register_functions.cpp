#include "register_functions.h"

#include "source/behaviors_shared/utilities.h"

#include "source/behaviors_shared/engine_functions.h"
#include "source/behaviors_shared/entities_functions.h"
#include "source/behaviors_shared/add_component_functions.h"
#include "source/behaviors_shared/components_functions.h"
#include "source/behaviors_shared/input_functions.h"
#include "source/behaviors_shared/require.h"

void behaviors::register_functions(lua_State* L)
{
	lua_functions::engine::register_functions(L);
	lua_functions::entities::register_functions(L);
	lua_functions::add_component::register_functions(L);
	lua_functions::components::register_functions(L);
	lua_functions::input::register_functions(L);
	lua_functions::require::register_functions(L);
}