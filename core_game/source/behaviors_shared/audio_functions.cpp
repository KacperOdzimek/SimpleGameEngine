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

			int _a_set_volume(lua_State* L)
			{
				auto precent = lua_tonumber(L, 1);
				common::audio_manager->set_volume(precent);
				return 0;
			}

			int _a_play_sound_at_channel(lua_State* L)
			{
				auto sound_path = load_asset_path(L, 1, "[_a_play_sound_at_channel]");
				uint32_t channel = load_id(L, 2, "[_a_play_sound_at_channel]", "Channel ");
				bool looping = lua_toboolean(L, 3);

				auto sound = assets::cast_asset<assets::sound>(common::assets_manager->safe_get_asset(sound_path));

				common::audio_manager->play_sound_at_channel(channel, sound, looping);

				return 0;
			}

			int _a_set_volume_at_channel(lua_State* L)
			{
				uint32_t channel = load_id(L, 1, "[_a_play_sound_at_channel]", "Channel ");
				float precent = lua_tonumber(L, 2);
				common::audio_manager->set_volume_at_channel(channel, precent);
				return 0;
			}

			int _a_resume_channel(lua_State* L)
			{
				uint32_t channel = load_id(L, 1, "[_a_play_sound_at_channel]", "Channel ");
				common::audio_manager->resume_sound_at_channel(channel);
				return 0;
			}

			int _a_stop_channel(lua_State* L)
			{
				uint32_t channel = load_id(L, 1, "[_a_play_sound_at_channel]", "Channel ");
				common::audio_manager->stop_sound_at_channel(channel);
				return 0;
			}

			void register_shared(lua_State* L)
			{
				lua_register(L, "_a_play_sound", _a_play_sound);
				lua_register(L, "_a_set_volume", _a_set_volume);
				lua_register(L, "_a_play_sound_at_channel", _a_play_sound_at_channel);
				lua_register(L, "_a_set_volume_at_channel", _a_set_volume_at_channel);
				lua_register(L, "_a_resume_channel", _a_resume_channel);
				lua_register(L, "_a_stop_channel", _a_stop_channel);
			}
		}
	}
}