#include "source/entities/entity.h"
#include "source/utilities/hash_string.h"

#include "source/components/behavior.h"
#include "source/assets/assets_manager.h"
#include "source/assets/asset.h"

#include "source/assets/behavior_asset.h"

namespace behaviors
{
	namespace lua_functions
	{
		namespace components
		{
			int _c_b_set_behavior(lua_State* L)
			{
				auto pointer = load_entity(L, 1);
				uint32_t comp = load_id(L, 2);
				uint32_t asset = load_id(L, 3);

				auto ptr = dynamic_cast<::entities::components::behavior*>(pointer->get_component(comp));
				if (ptr != nullptr)
					ptr->behavior_asset = ::assets::cast_asset<::assets::behavior>(::common::assets_manager->get_asset(asset));

				return 0;
			}

			void register_functions(lua_State* L)
			{
				lua_register(L, "_c_b_set_behavior", _c_b_set_behavior);
			}
		}
	}
}