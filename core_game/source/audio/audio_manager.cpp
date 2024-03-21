#include "audio_manager.h"

#include "source/common/common.h"
#include "source/common/crash.h"
#include "source/filesystem/filesystem.h"

using namespace audio;

#include "include/miniaudio/miniaudio.h"

#include <iostream>

struct audio_manager::implementation
{
    ma_engine engine;
};

audio_manager::audio_manager()
{
    impl = new implementation;

    ma_result result;

    result = ma_engine_init(NULL, &impl->engine);

    if (result != MA_SUCCESS)
        error_handling::crash(error_handling::error_source::core, "[audio_manager::audio_manager]",
            "Cannot initialize audio engine. Error code: " + std::to_string(result));
}

void audio_manager::play_sound(std::weak_ptr<assets::sound> sound)
{
    ma_result result = ma_engine_play_sound(&impl->engine, sound.lock()->file_path.c_str(), NULL);

    if (result != MA_SUCCESS)
        error_handling::crash(error_handling::error_source::core, "[audio_manager::play_sound]",
            "Cannot play file: " + sound.lock()->file_path + " Error code: " + std::to_string(result));
}

audio_manager::~audio_manager()
{
    ma_engine_uninit(&impl->engine);
    delete impl;
}