#include "audio_manager.h"

#include "source/common/common.h"
#include "source/common/crash.h"
#include "source/filesystem/filesystem.h"

using namespace audio;

#include "include/miniaudio/miniaudio.h"

#include <unordered_map>

struct channel_playback
{
    bool stoped = false;
    ma_sound sound;
};

struct audio_manager::implementation
{
    ma_engine engine;
    ma_device device;
    ma_sound_group group;

    std::unordered_map<uint32_t, channel_playback> channels;

    channel_playback& get_channel(const uint32_t& channel)
    {
        auto itr = channels.find(channel);
        if (itr == channels.end())
            error_handling::crash(error_handling::error_source::core, "[audio_manager::implementation::get_channel]",
                "Trying to use non-existent channel: " + std::to_string(channel));
        return channels.at(channel);
    };
};

audio_manager::audio_manager()
{
    impl = new implementation;

    ma_result result;

    auto engineConfig = ma_engine_config_init();

    result = ma_engine_init(&engineConfig, &impl->engine);
    if (result != MA_SUCCESS)
        error_handling::crash(error_handling::error_source::core, "[audio_manager::audio_manager]",
            "Cannot initialize audio engine. Error: " + std::to_string(result));

    result = ma_sound_group_init(&impl->engine, NULL, NULL, &impl->group);
    if (result != MA_SUCCESS)
        error_handling::crash(error_handling::error_source::core, "[audio_manager::audio_manager]",
            "Cannot initialize sounds group. Error: " + std::to_string(result));
}

void audio_manager::play_sound(std::weak_ptr<assets::sound> sound)
{
    ma_result result = ma_engine_play_sound(&impl->engine, sound.lock()->file_path.c_str(), &impl->group);

    if (result != MA_SUCCESS)
        error_handling::crash(error_handling::error_source::core, "[audio_manager::play_sound]",
            "Cannot play file: " + sound.lock()->file_path + " Error code: " + std::to_string(result));
}

void audio_manager::set_volume(float volume_precent)
{
    ma_sound_group_set_volume(&impl->group, volume_precent);
}

void audio_manager::play_sound_at_channel(uint32_t channel, std::weak_ptr<assets::sound> sound, bool looping)
{
    impl->channels.insert({ channel, {} });
    channel_playback& c = impl->get_channel(channel);

    ma_result result;

    result = ma_sound_init_from_file(&impl->engine, sound.lock()->file_path.c_str(), 0, &impl->group, NULL, &c.sound);
    if (result != MA_SUCCESS) {
        error_handling::crash(error_handling::error_source::core, "[audio_manager::play_sound_at_channel]",
            "Cannot play file: " + sound.lock()->file_path + " Error code: " + std::to_string(result));
    }

    ma_sound_start(&c.sound);
    ma_sound_set_looping(&c.sound, looping);
}

void audio_manager::set_volume_at_channel(uint32_t channel, float volume_precent)
{
    channel_playback& c = impl->get_channel(channel);
    ma_sound_set_volume(&c.sound, volume_precent);
}

void audio_manager::resume_sound_at_channel(uint32_t channel)
{
    channel_playback& c = impl->get_channel(channel);
    if (c.stoped && (!ma_sound_at_end(&c.sound) || ma_sound_is_looping(&c.sound)))  //prevents unintentional looping
    {
        ma_sound_start(&c.sound);
        c.stoped = false;
    }   
}

void audio_manager::stop_sound_at_channel(uint32_t channel)
{
    channel_playback& c = impl->get_channel(channel);
    ma_sound_stop(&c.sound);
    c.stoped = true;
}

audio_manager::~audio_manager()
{
    ma_engine_uninit(&impl->engine);
    delete impl;
}