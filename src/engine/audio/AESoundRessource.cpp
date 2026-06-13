#include "gof2/engine/audio/AESoundRessource.h"

extern "C" char AESoundInterface_vtable[];

typedef void (*SoundVoidMethod)(void *);
typedef void (*SoundIntMethod)(void *, int);
typedef void (*SoundFloatMethod)(void *, float);
typedef void (*SoundPointerMethod)(void *, const char *);
typedef int (*SoundIntReturnMethod)(void *);

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
        void *sound = (*sounds)[i];
        if (sound != nullptr)
            ::operator delete(sound);
        (*sounds)[i] = nullptr;
    }
    delete sounds;
}

void AESoundRessource::freeAllRessources()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        void *sound = (*this->sounds)[i];
        if (sound != 0) {
            ::operator delete(sound);
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
        void *sound = (*this->sounds)[index];
        if (sound == 0) {
            sound = ::operator new(4);
            *(void **)sound = AESoundInterface_vtable + 8;
            (*this->sounds)[index] = (AESoundInterface *)sound;
            sound = (*this->sounds)[index];
        }
        (*(SoundPointerMethod *)*(void **)sound)(sound, *(const char **)(frame.info + 4));
        sound = (*this->sounds)[index];
        (*(SoundIntMethod *)((char *)*(void **)sound + 0x24))(sound, *(int *)(frame.info + 8));
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
            void *sound = ::operator new(4);
            *(void **)sound = AESoundInterface_vtable + 8;
            (*this->sounds)[index] = (AESoundInterface *)sound;
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
        void *sound = (*this->sounds)[index];
        if ((*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x38))(sound) == 0) {
            this->init(savedId);
        }
        sound = (*this->sounds)[index];
        if (sound != 0) {
            (*(SoundFloatMethod *)((char *)*(void **)sound + 0xc))(sound, savedVolume);
        }
    }
}

void AESoundRessource::playLoop(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = (*this->sounds)[index];
        if (sound != 0 && (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound) == 0) {
            sound = (*this->sounds)[index];
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x10))(sound);
        }
    }
}

void AESoundRessource::playMusic(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = (*this->sounds)[index];
        if (sound != 0 && (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound) == 0) {
            sound = (*this->sounds)[index];
            (*(SoundVoidMethod *)((char *)*(void **)sound + 8))(sound);
        }
    }
}

void AESoundRessource::playMusicLoop(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    int index = frame.index;
    if (index != -1) {
        void *sound = (*this->sounds)[index];
        if (sound != 0 && (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound) == 0) {
            sound = (*this->sounds)[index];
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x10))(sound);
        }
    }
}

// ---- stop / pause / resume / suspend ---------------------------------------
void AESoundRessource::stop(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = (*this->sounds)[frame.index];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x1c))(sound);
        }
    }
}

void AESoundRessource::stopAll()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        void *sound = (*this->sounds)[i];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x1c))(sound);
        }
    }
}

void AESoundRessource::pause(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = (*this->sounds)[frame.index];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x14))(sound);
        }
    }
}

int AESoundRessource::pauseAll()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        void *sound = (*this->sounds)[i];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x14))(sound);
        }
    }
    return this->sounds->size();
}

void AESoundRessource::resume(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = (*this->sounds)[frame.index];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x18))(sound);
        }
    }
}

void AESoundRessource::resumeAll()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        void *sound = (*this->sounds)[i];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 0x18))(sound);
        }
    }
}

void AESoundRessource::suspendAll()
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        void *sound = (*this->sounds)[i];
        if (sound != 0) {
            (*(SoundVoidMethod *)((char *)*(void **)sound + 4))(sound);
        }
    }
}

void AESoundRessource::release(int id)
{
    AESoundStackFrame frame;
    this->getSoundInfo(id, (AESoundInfo *)frame.info, &frame.index);
    if (frame.index != -1) {
        void *sound = (*this->sounds)[frame.index];
        (*(SoundVoidMethod *)((char *)*(void **)sound + 0x34))(sound);
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
        void *sound = (*this->sounds)[frame.index];
        result = (*(SoundIntReturnMethod *)((char *)*(void **)sound + 0x20))(sound);
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
        void *sound = (*this->sounds)[frame.index];
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x28))(sound, savedVolume);
        }
    }
}

void AESoundRessource::setVolume(int volume)
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        void *sound = (*this->sounds)[i];
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x28))(sound, volume);
        }
    }
}

void AESoundRessource::setSoundVolume(int volume)
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        void *sound = (*this->sounds)[i];
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x2c))(sound, volume);
        }
    }
}

void AESoundRessource::setMusicVolume(int volume)
{
    for (uint32_t i = 0; i < this->sounds->size(); ++i) {
        void *sound = (*this->sounds)[i];
        if (sound != 0) {
            (*(SoundIntMethod *)((char *)*(void **)sound + 0x30))(sound, volume);
        }
    }
}

} // namespace AbyssEngine
