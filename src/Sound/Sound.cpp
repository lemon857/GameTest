#include "EngineCore/Sound/Sound.h"
#include "EngineCore/Sound/SoundEngine.h"

#define MA_NO_WAV
#define MA_NO_MP3
#define MA_NO_ENCODING
#define MINIAUDIO_IMPLEMENTATION
#include "EngineCore/Sound/miniaudio.h"

Sound::Sound()
{
    m_sound = new ma_sound();
}

Sound::~Sound()
{
    ma_sound_stop(m_sound);
    delete m_sound;
}

int Sound::init(const char* path)
{
    ma_result result;

    result = ma_sound_init_from_file(SoundEngine::get_engine(), path, MA_SOUND_FLAG_DECODE, NULL, NULL, m_sound);
    if (result != MA_SUCCESS) {
        return result;  // Failed to initialize the engine.
    }
    ma_sound_set_pinned_listener_index(m_sound, 0);

    return 0;
}

int Sound::play()
{
    ma_result result;
    result = ma_sound_start(m_sound);
    if (result != MA_SUCCESS) {
        return result;  // Failed to initialize the engine.
    }
    return 0;
}

void Sound::set_position(float x, float y, float z)
{
    ma_sound_set_position(m_sound, x, y, z);
}

void Sound::set_direction(float x, float y, float z)
{
    ma_sound_set_direction(m_sound, x, y, z);
}

void Sound::set_voulme(float voulme)
{
    ma_sound_set_volume(m_sound, voulme);
}

void Sound::set_looping(bool looping)
{
    ma_sound_set_looping(m_sound, looping);
}

void Sound::set_positioning(bool isAbsolute)
{
    ma_sound_set_positioning(m_sound, isAbsolute ? ma_positioning_absolute : ma_positioning_relative);
}
