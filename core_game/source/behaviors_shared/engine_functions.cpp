#include "engine_functions.h"

#include "source/behaviors_shared/utilities.h"

#include "source/common/common.h"
#include "source/entities/world.h"

namespace behaviors
{
	namespace lua_shared
	{
		namespace engine
		{
			int _en_load_scene(lua_State* L)
			{
				auto name = load_id(L, 1, "[_en_load_scene]", "Scene");
				auto scene_asset = load_asset_path(L, 2, "[_en_load_scene]");

				common::world->create_scene(
					name,
					assets::cast_asset<assets::scene>(common::assets_manager->safe_get_asset(scene_asset))
				);

				return 0;
			}

			int _en_unload_scene(lua_State* L)
			{
				auto name = load_id(L, 1, "[_en_load_scene]", "Scene");

				common::world->remove_scene(name);

				return 0;
			}

			void register_shared(lua_State* L)
			{
				lua_register(L, "_en_load_scene", _en_load_scene);
				lua_register(L, "_en_unload_scene", _en_unload_scene);
			}
		}
	}
}