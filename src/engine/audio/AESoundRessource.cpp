#include "gof2/engine/audio/AESoundRessource.h"


static const char defaultSoundName[] = "";

namespace AbyssEngine {

// Constructs the AESoundInterface array, then zeroes the count and info-table pointer.
AESoundRessource::AESoundRessource()
{
    this->sounds = new Array<AESoundInterface *>();
    this->numSounds = 0;
    this->soundInfoTable = 0;
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

void AESoundRessource::SetSound(AESoundInfo *info, int count)
{
    this->numSounds = count;
    this->soundInfoTable = (char *)info;
    this->sounds->resize(count);
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        (*this->sounds)[i] = nullptr;
    }
}

void AESoundRessource::getSoundInfo(int id, AESoundInfo *info, int *index)
{
    *index = -1;
    uint32_t count = this->sounds->size();
    for (uint32_t i = 0; i < count; ++i) {
        char *base = this->soundInfoTable;
        AESoundInfo *entry = (AESoundInfo *)(base + i * 12);
        if (entry->id == id) {
            *index = i;
            *info = *entry;
            return;
        }
    }
}

void AESoundRessource::init(int id)
{
    AESoundStackFrame frame;
    const char *name = defaultSoundName;
    *(const char * volatile *)(frame.info + 4) = name;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound == nullptr) {
            sound = new AESoundInterface();
            (*this->sounds)[index] = sound;
        }
        sound->load(*(const char **)(frame.info + 4));
        sound->setType(*(int *)(frame.info + 8));
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
    AESoundStackFrame frame;
    const char *name = defaultSoundName;
    *(const char * volatile *)(frame.info + 4) = name;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        if ((*this->sounds)[index] == nullptr) {
            (*this->sounds)[index] = new AESoundInterface();
        }
    }
}

void AESoundRessource::play(int id, float volume)
{
    float savedVolume = volume;
    int savedId = id;
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound->isLoaded() == 0) {
            this->init(savedId);
        }
        sound = (*this->sounds)[index];
        if (sound != nullptr) {
            sound->play(savedVolume);
        }
    }
}

void AESoundRessource::playLoop(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound != nullptr && sound->isPlaying() == 0) {
            sound = (*this->sounds)[index];
            sound->playLooping();
        }
    }
}

void AESoundRessource::playMusic(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound != nullptr && sound->isPlaying() == 0) {
            sound = (*this->sounds)[index];
            sound->play();
        }
    }
}

void AESoundRessource::playMusicLoop(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        AESoundInterface *sound = (*this->sounds)[index];
        if (sound != nullptr && sound->isPlaying() == 0) {
            sound = (*this->sounds)[index];
            sound->playLooping();
        }
    }
}

// ---- stop / pause / resume / suspend ---------------------------------------
void AESoundRessource::stop(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        AESoundInterface *sound = (*this->sounds)[frame.index];
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
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        AESoundInterface *sound = (*this->sounds)[frame.index];
        if (sound != nullptr) {
            sound->pause();
        }
    }
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
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        AESoundInterface *sound = (*this->sounds)[frame.index];
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
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        AESoundInterface *sound = (*this->sounds)[frame.index];
        sound->release();
    }
}

bool AESoundRessource::isPlaying(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    int result;
    if (frame.index == -1) {
        result = 0;
    } else {
        AESoundInterface *sound = (*this->sounds)[frame.index];
        result = sound->isPlaying();
    }
    return result != 0;
}

// ---- volume control --------------------------------------------------------
void AESoundRessource::setVolume(int id, int volume)
{
    int savedVolume = volume;
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        AESoundInterface *sound = (*this->sounds)[frame.index];
        if (sound != nullptr) {
            sound->setVolume(savedVolume);
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
