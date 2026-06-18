#include "engine/audio/AESoundRessource.h"

static const char defaultSoundName[] = "";

namespace AbyssEngine {

// Constructs the AESoundInterface array, then zeroes the count and info-table pointer.
AESoundRessource::AESoundRessource()
{
    this->sounds = new Array<AESoundInterface *>();
    this->numSounds = 0;
    this->soundInfoTable = nullptr;
}

AESoundRessource::~AESoundRessource()
{
    freeAllRessources();
    for (uint32_t i = 0; i < sounds->size(); ++i) {
        AESoundInterface *sound = (*sounds)[i];
        if (sound != nullptr)
            delete sound;
        (*sounds)[i] = nullptr;
    }
    delete sounds;
}

void AESoundRessource::freeAllRessources()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        AESoundInterface *sound = (*this->sounds)[i];
        if (sound != nullptr) {
            delete sound;
            (*this->sounds)[i] = nullptr;
        }
    }
}

void AESoundRessource::SetSound(const AESoundInfo *info, int count)
{
    this->numSounds = count;
    this->soundInfoTable = info;
    this->sounds->resize(count);
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        (*this->sounds)[i] = nullptr;
    }
}

void AESoundRessource::getSoundInfo(int id, AESoundInfo &info, int &index)
{
    index = -1;
    uint32_t count = this->sounds->size();
    for (uint32_t i = 0; i < count; ++i) {
        const AESoundInfo *entry = &this->soundInfoTable[i];
        if (entry->id == id) {
            index = i;
            info = *entry;
            return;
        }
    }
}

void AESoundRessource::init(int id)
{
    AESoundInfo info;
    info.name = defaultSoundName;
    int index;
    this->getSoundInfo(id, info, index);
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound == nullptr) {
            sound = new AESoundInterface();
            (*this->sounds)[index] = sound;
        }
        sound->load(info.name);
        sound->setType(info.type);
    }
}

void AESoundRessource::play_impl(int id, int volume)
{
    play(id, (float)volume);
}

// Empty in this build (the looping bookkeeping was compiled out).
void AESoundRessource::checkLooping()
{
}

// ---- one-shot / loop playback ----------------------------------------------
void AESoundRessource::initWithoutLoading(int id)
{
    AESoundInfo info;
    info.name = defaultSoundName;
    int index;
    this->getSoundInfo(id, info, index);
    if (index != -1) {
        if ((*this->sounds)[index] == nullptr) {
            (*this->sounds)[index] = new AESoundInterface();
        }
    }
}

void AESoundRessource::play(int id, float volume)
{
    AESoundInfo info;
    int index;
    this->getSoundInfo(id, info, index);
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound->isLoaded() == 0) {
            this->init(id);
        }
        sound = (*this->sounds)[index];
        if (sound != nullptr) {
            sound->play(volume);
        }
    }
}

void AESoundRessource::play(int id)
{
    play(id, 0.0f);
}

void AESoundRessource::playLoop(int id)
{
    AESoundInfo info;
    int index;
    this->getSoundInfo(id, info, index);
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound != nullptr && sound->isPlaying() == 0) {
            sound->playLooping();
        }
    }
}

void AESoundRessource::playMusic(int id)
{
    AESoundInfo info;
    int index;
    this->getSoundInfo(id, info, index);
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound != nullptr && sound->isPlaying() == 0) {
            sound->play();
        }
    }
}

void AESoundRessource::playMusicLoop(int id)
{
    AESoundInfo info;
    int index;
    this->getSoundInfo(id, info, index);
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound != nullptr && sound->isPlaying() == 0) {
            sound->playLooping();
        }
    }
}

// ---- stop / pause / resume / suspend ---------------------------------------
void AESoundRessource::stop(int id)
{
    AESoundInfo info;
    int index;
    this->getSoundInfo(id, info, index);
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound != nullptr) {
            sound->stop();
        }
    }
}

void AESoundRessource::stop()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        AESoundInterface *sound = (*this->sounds)[i];
        if (sound != nullptr) {
            sound->stop();
        }
    }
}

void AESoundRessource::stopAll()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        AESoundInterface *sound = (*this->sounds)[i];
        if (sound != nullptr) {
            sound->stop();
        }
    }
}

void AESoundRessource::pause(int id)
{
    AESoundInfo info;
    int index;
    this->getSoundInfo(id, info, index);
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound != nullptr) {
            sound->pause();
        }
    }
}

int AESoundRessource::pause()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        AESoundInterface *sound = (*this->sounds)[i];
        if (sound != nullptr) {
            sound->pause();
        }
    }
    return this->sounds->size();
}

int AESoundRessource::pauseAll()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        AESoundInterface *sound = (*this->sounds)[i];
        if (sound != nullptr) {
            sound->pause();
        }
    }
    return this->sounds->size();
}

void AESoundRessource::resume(int id)
{
    AESoundInfo info;
    int index;
    this->getSoundInfo(id, info, index);
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound != nullptr) {
            sound->resume();
        }
    }
}

void AESoundRessource::resume()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        AESoundInterface *sound = (*this->sounds)[i];
        if (sound != nullptr) {
            sound->resume();
        }
    }
}

void AESoundRessource::resumeAll()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        AESoundInterface *sound = (*this->sounds)[i];
        if (sound != nullptr) {
            sound->resume();
        }
    }
}

void AESoundRessource::suspendAll()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        AESoundInterface *sound = (*this->sounds)[i];
        if (sound != nullptr) {
            sound->suspend();
        }
    }
}

void AESoundRessource::release(int id)
{
    AESoundInfo info;
    int index;
    this->getSoundInfo(id, info, index);
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        sound->release();
    }
}

bool AESoundRessource::isPlaying(int id)
{
    AESoundInfo info;
    int index;
    this->getSoundInfo(id, info, index);
    int result;
    if (index == -1) {
        result = 0;
    } else {
        AESoundInterface *sound = (*this->sounds)[index];
        result = sound->isPlaying();
    }
    return result != 0;
}

// ---- volume control --------------------------------------------------------
void AESoundRessource::setVolume(int id, int volume)
{
    AESoundInfo info;
    int index;
    this->getSoundInfo(id, info, index);
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound != nullptr) {
            sound->setVolume(volume);
        }
    }
}

void AESoundRessource::setVolume(int volume)
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        AESoundInterface *sound = (*this->sounds)[i];
        if (sound != nullptr) {
            sound->setVolume(volume);
        }
    }
}

void AESoundRessource::setSoundVolume(int volume)
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        AESoundInterface *sound = (*this->sounds)[i];
        if (sound != nullptr) {
            sound->setSoundVolume(volume);
        }
    }
}

void AESoundRessource::setMusicVolume(int volume)
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        AESoundInterface *sound = (*this->sounds)[i];
        if (sound != nullptr) {
            sound->setMusicVolume(volume);
        }
    }
}

} // namespace AbyssEngine
