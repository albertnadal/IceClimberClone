#include <sound_manager.h>
#include <iostream>
#include <sstream>
#include <iomanip>

SoundManager::SoundManager() {
        LoadSounds();
}

SoundManager::~SoundManager() {
        UnloadSounds();
}

void SoundManager::LoadSounds()
{
    for (uint16_t i = 0; i < static_cast<uint16_t>(SoundIdentificator::MAX_SOUND_IDENTIFICATORS); ++i) {
        std::stringstream ss;
        ss << AUDIO_FOLDER << std::setw(2) << std::setfill('0') << i << ".wav";
        std::string filename = ss.str();

        Sound sound = LoadSound(filename.c_str());
        if (sound.stream.buffer == nullptr) {
            std::cerr << "Error loading sound file: " << filename << std::endl;
            continue;
        }

        soundsMap[static_cast<SoundIdentificator>(i)] = sound;
    }
}

void SoundManager::UnloadSounds()
{
    for (auto& pair : soundsMap) {
        UnloadSound(pair.second);
    }
}

void SoundManager::PlaySoundById(SoundIdentificator soundId) const {
    auto it = soundsMap.find(soundId);
    if (it != soundsMap.end()) {
        PlaySound(it->second);
    }
}
