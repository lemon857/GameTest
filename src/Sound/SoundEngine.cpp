#include "EngineCore/Sound/SoundEngine.h"

#define MA_NO_WAV
#define MA_NO_MP3
#define MA_NO_ENCODING
#define MINIAUDIO_IMPLEMENTATION
#include "EngineCore/Sound/miniaudio.h"

ma_engine* SoundEngine::m_soundEngine;

int SoundEngine::init_audio()
{
    ma_result result;
    ma_engine_config engineConfig;
    m_soundEngine = new ma_engine();

    result = ma_engine_init(NULL, m_soundEngine);
    if (result != MA_SUCCESS) {
        return result;  // Failed to initialize the engine.
    }

    engineConfig = ma_engine_config_init();
    engineConfig.listenerCount = 1;
    engineConfig.noAutoStart = MA_TRUE;
    engineConfig.noDevice = MA_TRUE;
    engineConfig.channels = 2;        // Must be set when not using a device.
    engineConfig.sampleRate = 48000;    // Must be set when not using a device.

    result = ma_engine_init(&engineConfig, m_soundEngine);
    if (result != MA_SUCCESS) {
        return result;  // Failed to initialize the engine.
    }
    ma_engine_start(m_soundEngine);

    ma_engine_listener_set_world_up(m_soundEngine, 0, 0, 1, 0);
    //ma_engine_listener_set_cone(&engine, 0, 1, 2, 3);       

    return 0;
}

void SoundEngine::uninit_audio()
{
    ma_engine_stop(m_soundEngine);
    delete m_soundEngine;
}

void SoundEngine::set_position(float x, float y, float z)
{
    ma_engine_listener_set_position(m_soundEngine, 0, x, y, z);
}

void SoundEngine::set_direction(float x, float y, float z)
{
    ma_engine_listener_set_direction(m_soundEngine, 0, x, y, z);
}

ma_engine* SoundEngine::get_engine()
{
    return m_soundEngine;
}
