#include "source/assets/assets_manager.h"
#include "source/common/common.h"

#include "source/assets/behavior_asset.h"
#include "source/components/behavior.h"

namespace behaviors
{
	namespace lua_functions
	{
		namespace add_component
		{
			int _e_add_behavior(lua_State* L)
			{
				::entities::entity* e = load_entity(L, 1);
				uint32_t id = load_id(L, 2);
				uint32_t bhv = load_id(L, 3);

				e->attach_component(
					new ::entities::components::behavior{
						id,
						::assets::cast_asset<::assets::behavior>(::common::assets_manager->get_asset(bhv))
					}
				);

				return 0;
			}

			void register_functions(lua_State* L)
			{
				lua_register(L, "_e_add_behavior", _e_add_behavior);
			}
		}
	}
}