#include "source/assets/asset.h"
#include "source/assets/assets_manager.h"
#include "source/common/common.h"
#include "source/utilities/hash_string.h"

namespace behaviors
{
	namespace functions
	{
		namespace assets
		{
			int _a_get(lua_State* L)
			{
				auto name = lua_tostring(L, 1);
				auto _asset = common::assets_manager->get_asset(utilities::hash_string(name));
				lua_pushinteger(L, (uint64_t)_asset.lock().get());
				return 1;
			}

			void register_functions(lua_State* L)
			{
				lua_register(L, "_a_get", _a_get);
			}
		}
	}
}