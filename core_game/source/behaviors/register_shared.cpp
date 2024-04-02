#include "register_shared.h"

#include "source/behaviors_shared/utilities.h"

#include "source/behaviors_shared/engine_functions.h"
#include "source/behaviors_shared/entity_reference.h"
#include "source/behaviors_shared/require.h"
#include "source/behaviors_shared/audio_functions.h"
#include "source/behaviors_shared/entities_functions.h"
#include "source/behaviors_shared/add_component_functions.h"
#include "source/behaviors_shared/components_functions.h"
#include "source/behaviors_shared/input_functions.h"
#include "source/behaviors_shared/collision_functions.h"

void behaviors::register_shared(lua_State* L)
{
	lua_shared::engine::register_shared(L);
	lua_shared::entity_reference::register_shared(L);
	lua_shared::require::register_shared(L);
	lua_shared::audio::register_shared(L);
	lua_shared::entities::register_shared(L);
	lua_shared::add_component::register_shared(L);
	lua_shared::components::register_shared(L);
	lua_shared::input::register_shared(L);
	lua_shared::collision::register_shared(L);
}