#include "audio_functions.h"

#include "utilities.h"

#include "source/common/common.h"
#include "source/audio/audio_manager.h"

#include "source/assets/sound_asset.h"

namespace behaviors
{
	namespace lua_shared
	{
		namespace audio
		{
			int _a_play_sound(lua_State* L)
			{
				auto sound_path = load_asset_path(L, 1, "[_a_play_sound]");
				auto sound = assets::cast_asset<assets::sound>(common::assets_manager->safe_get_asset(sound_path));

				common::audio_manager->play_sound(sound);

				return 0;
			}

			void register_shared(lua_State* L)
			{
				lua_register(L, "_a_play_sound", _a_play_sound);
			}
		}
	}
}