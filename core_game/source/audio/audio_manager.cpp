#include "audio_manager.h"

#include "source/common/common.h"
#include "source/common/crash.h"
#include "source/filesystem/filesystem.h"

#include "source/entities/entity.h"
#include "source/components/listener.h"
#include "source/components/sound_emitter.h"

#include "include/miniaudio/miniaudio.h"

#include <list>
#include <unordered_map>

using namespace audio;

struct channel_playback
{
    bool stoped = false;
    ma_sound sound{};
};

struct emitter_sounds
{
    std::list<ma_sound*> sounds{};
};

using sound_emitter = entities::components::sound_emitter;

template<>
struct std::hash<sound_emitter*>
{
    size_t operator() (sound_emitter* a) const
    {
        return (size_t)a;
    }
};

template<>
struct std::equal_to<sound_emitter*>
{
    bool operator() (sound_emitter* const& a, sound_emitter* const& b) const
    {
        return a == b;
    }
};

bool operator==(std::pair<sound_emitter*, emitter_sounds> a, sound_emitter* b)
{
    return a.first == b;
}

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

    float desired_volume = 1;
    float rolloff;

    entities::components::listener* active_listener = nullptr;

    std::unordered_map<sound_emitter*, emitter_sounds> emitters;
};

audio_manager::audio_manager()
{
    impl = new implementation;

    ma_result result;

    auto engineConfig = ma_engine_config_init();
    engineConfig.listenerCount = 1;

    result = ma_engine_init(&engineConfig, &impl->engine);
    if (result != MA_SUCCESS)
        error_handling::crash(error_handling::error_source::core, "[audio_manager::audio_manager]",
            "Cannot initialize audio engine. Error: " + std::to_string(result));

    result = ma_sound_group_init(&impl->engine, NULL, NULL, &impl->group);
    if (result != MA_SUCCESS)
        error_handling::crash(error_handling::error_source::core, "[audio_manager::audio_manager]",
            "Cannot initialize sounds group. Error: " + std::to_string(result));

    if (impl->active_listener == nullptr)
    {
        ma_sound_group_set_volume(&impl->group, 0);
        return;
    }
}

audio_manager::~audio_manager()
{
    for (auto& channel : impl->channels)
        ma_sound_uninit(&channel.second.sound);

    ma_engine_uninit(&impl->engine);

    delete impl;
}

void audio_manager::set_audio_rolloff(float rolloff)
{
    impl->rolloff = rolloff;
}

void audio_manager::update()
{
    if (impl->active_listener == nullptr)
    {
        ma_sound_group_set_volume(&impl->group, 0);
        return;
    }

    ma_sound_group_set_volume(&impl->group, impl->desired_volume);

    auto e = impl->active_listener->get_owner_weak().lock();

    ma_engine_listener_set_position(
        &impl->engine, 
        0, 
        e->get_location().x,
        e->get_location().y,
        e->layer
    );

    for (auto& emitter : impl->emitters)
    {
        auto owner = emitter.first->get_owner_weak().lock();

        auto& sounds = emitter.second.sounds;

        auto i = sounds.begin();
        while (i != sounds.end())
        {
            ma_sound_set_position(
                *i,
                owner->get_location().x, 
                owner->get_location().y,
                owner->layer
            );

            if (ma_sound_at_end(*i))
                sounds.erase(i++);
            else
                i++;
        }
    }       
}

bool audio_manager::is_active_listner(entities::components::listener* listener)
{
    return impl->active_listener == listener;
}

void audio_manager::set_active_listener(entities::components::listener* active_listener)
{
    impl->active_listener = active_listener;
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
    impl->desired_volume = volume_precent;
}

void audio_manager::play_sound_at_channel(uint32_t channel, std::weak_ptr<assets::sound> sound, bool looping)
{
    if (impl->channels.find(channel) != impl->channels.end())
        ma_sound_uninit(&impl->channels.at(channel).sound);
    else
        impl->channels.insert({ channel, {} });

    channel_playback& c = impl->get_channel(channel);

    ma_result result;

    result = ma_sound_init_from_file(
        &impl->engine, 
        sound.lock()->file_path.c_str(), 
        MA_SOUND_FLAG_ASYNC, 
        &impl->group, 
        NULL, 
        &c.sound
    );

    ma_sound_set_spatialization_enabled(&c.sound, false);

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

void audio_manager::set_position_at_channel(uint32_t channel, glm::vec3 position)
{
    channel_playback& c = impl->get_channel(channel);
    ma_sound_set_position(&c.sound, position.x, position.y, position.z);
    ma_sound_set_rolloff(&c.sound, impl->rolloff);
    ma_sound_set_spatialization_enabled(&c.sound, true);
}

void audio_manager::register_emitter(sound_emitter* emitter)
{
    impl->emitters.insert({emitter, {}});
}

void audio_manager::unregister_emitter(sound_emitter* emitter)
{
    auto itr = std::find(impl->emitters.begin(), impl->emitters.end(), emitter);

    for (auto& sound : itr->second.sounds)
        ma_sound_uninit(sound);

    impl->emitters.erase(itr);
}

void audio_manager::emit_sound(sound_emitter* emitter, std::weak_ptr<assets::sound> sound, float volume_precent)
{
    ma_result result;
    auto ptr = new ma_sound;

    result = ma_sound_init_from_file(
        &impl->engine,
        sound.lock()->file_path.c_str(),
        MA_SOUND_FLAG_ASYNC,
        &impl->group,
        NULL,
        ptr
    );

    if (result != MA_SUCCESS) {
        error_handling::crash(error_handling::error_source::core, "[audio_manager::emit_sound]",
            "Cannot play file: " + sound.lock()->file_path + " Error code: " + std::to_string(result));
    }

    auto owner = emitter->get_owner_weak().lock();

    ma_sound_set_position(
        ptr,
        owner->get_location().x,
        owner->get_location().y,
        owner->layer
    );

    ma_sound_set_rolloff(ptr, impl->rolloff);
    ma_sound_set_volume(ptr, volume_precent);
    ma_sound_start(ptr);

    impl->emitters.at(emitter).sounds.push_back(ptr);
}